#include "PrecompiledHeader.h"

#include "App.h"

#include "KeyMovieOnFile.h"

#define HEADER_SIZE (sizeof(KeyMovieHeader)+4+4)
#define SAVESTATE_HEADER_SIZE (sizeof(bool) + sizeof(savestate.savestatesize) + sizeof(savestate.savestate[0]) * savestate.savestatesize)
#define BLOCK_HEADER_SIZE (0) 
#define BLOCK_DATA_SIZE (18*2)
#define BLOCK_SIZE (BLOCK_HEADER_SIZE+BLOCK_DATA_SIZE)

#define SEEKPOINT_FRAMEMAX (sizeof(KeyMovieHeader))
#define SEEKPOINT_UNDOCOUNT (sizeof(KeyMovieHeader)+4)
#define SEEKPOINT_SAVESTATE (SEEKPOINT_UNDOCOUNT+4)

long KeyMovieOnFile::_getBlockSeekPoint(const long & frame)
{
	if (savestate.fromSavestate) {
		return HEADER_SIZE
			+ SAVESTATE_HEADER_SIZE
			+ frame * BLOCK_SIZE;
	}
	else {
		return HEADER_SIZE + sizeof(bool) + (frame)*BLOCK_SIZE;
	}
}

//----------------------------------
// file
//----------------------------------
bool KeyMovieOnFile::Open(const wxString fn, bool fNewOpen, VmStateBuffer *ss)
{
	Close();
	wxString mode = L"rb+";
	if (fNewOpen) {
		mode = L"wb+";
		MaxFrame = 0;
		UndoCount = 0;
		header.init();
	}
	if ( fopen_s(&fp, fn.c_str(), mode.c_str()) != 0)
	{
		Console.WriteLn(Color_StrongBlue, "[KeyMovie]file open fail: %s", strerror(errno));
		return false;
	}
	filename = fn;

	if (fNewOpen) {
		if (ss) {
			savestate.fromSavestate = true;
			savestate.savestatesize = ss->GetLength();
			savestate.savestate.MakeRoomFor(ss->GetLength());
			for (size_t i = 0; i < ss->GetLength(); i++) {
				savestate.savestate[i] = (*ss)[i];
			}
		}
		else {
			// TODO fix this UI_DisableSysReset();
			sApp.SysExecute();
		}
	}
	return true;
}
bool KeyMovieOnFile::Close()
{
	if (fp == NULL)return false;
	writeHeader();
	writeSavestate();
	fclose(fp);
	fp = NULL;
	filename = "";
	return true;
}

//----------------------------------
// write frame
//----------------------------------
bool KeyMovieOnFile::writeKeyBuf(const uint & frame, const uint port, const uint bufIndex, const u8 & buf)
{
	if (fp == NULL)return false;

	long seek = _getBlockSeekPoint(frame) + BLOCK_HEADER_SIZE + 18 * port + bufIndex;
	if (fseek(fp, seek, SEEK_SET) != 0){
		return false;
	}
	if (fwrite(&buf, 1, 1, fp) != 1) {
		return false;
	}
	fflush(fp);
	return true;
}

//----------------------------------
// read frame
//----------------------------------
bool KeyMovieOnFile::readKeyBuf(u8 & result,const uint & frame, const uint port, const uint  bufIndex)
{
	if (fp == NULL)return false;

	long seek = _getBlockSeekPoint(frame) + BLOCK_HEADER_SIZE + 18 * port + bufIndex;
	if (fseek(fp, seek, SEEK_SET) != 0) {
		return false;
	}
	if (fread(&result, 1, 1, fp) != 1) {
		return false;
	}
	return true;
}




//===================================
// pad
//===================================
void KeyMovieOnFile::getPadData(PadData & result, unsigned long frame)
{
	result.fExistKey = false;
	if (fp == NULL)return;
	long seek = _getBlockSeekPoint(frame) + BLOCK_HEADER_SIZE;
	if (fseek(fp, seek, SEEK_SET) != 0)return;
	if (fread(result.buf, 1, BLOCK_DATA_SIZE, fp) == 0)return;
	result.fExistKey = true;
}
bool KeyMovieOnFile::DeletePadData(unsigned long frame)
{
	if (fp == NULL)return false;

	for (unsigned long i = frame; i < MaxFrame - 1; i++)
	{
		long seek1 = _getBlockSeekPoint(i+1) + BLOCK_HEADER_SIZE;
		long seek2 = _getBlockSeekPoint(i) + BLOCK_HEADER_SIZE;

		u8 buf[2][18];
		fseek(fp, seek1, SEEK_SET);
		fread(buf, 1, BLOCK_DATA_SIZE, fp);
		fseek(fp, seek2, SEEK_SET);
		fwrite(buf,1, BLOCK_DATA_SIZE, fp);
	}
	MaxFrame--;
	writeMaxFrame();
	fflush(fp);

	return true;
}
bool KeyMovieOnFile::InsertPadData(unsigned long frame, const PadData& key)
{
	if (fp == NULL)return false;
	if (!key.fExistKey)return false;

	for (unsigned long i = MaxFrame - 1; i >= frame; i--)
	{
		long seek1 = _getBlockSeekPoint(i) + BLOCK_HEADER_SIZE;
		long seek2 = _getBlockSeekPoint(i+1) + BLOCK_HEADER_SIZE;

		u8 buf[2][18];
		fseek(fp, seek1, SEEK_SET);
		fread(buf, 1, BLOCK_DATA_SIZE, fp);
		fseek(fp, seek2, SEEK_SET);
		fwrite(buf, 1, BLOCK_DATA_SIZE, fp);
	}
	{
		long seek = _getBlockSeekPoint(frame) + BLOCK_HEADER_SIZE;
		fseek(fp, seek, SEEK_SET);
		fwrite(key.buf, 1, BLOCK_DATA_SIZE, fp);
	}
	MaxFrame++;
	writeMaxFrame();
	fflush(fp);

	return true;
}
bool KeyMovieOnFile::UpdatePadData(unsigned long frame, const PadData& key)
{
	if (fp == NULL)return false;
	if (!key.fExistKey)return false;

	long seek = _getBlockSeekPoint(frame) + BLOCK_HEADER_SIZE;
	fseek(fp, seek, SEEK_SET);
	if (fwrite(key.buf, 1, BLOCK_DATA_SIZE, fp) == 0)return false;

	fflush(fp);
	return true;
}




//===================================
// header
//===================================
bool KeyMovieOnFile::readHeaderAndCheck()
{
	if (fp == NULL)return false;
	rewind(fp);
	if (fread(&header, sizeof(KeyMovieHeader), 1, fp) != 1)return false;
	if (fread(&MaxFrame, 4, 1, fp) != 1)return false;
	if (fread(&UndoCount, 4, 1, fp) != 1)return false;
	if (fread(&savestate.fromSavestate, sizeof(bool), 1, fp) != 1) return false;
	if (savestate.fromSavestate) {
		// We read the size (and the savestate) only if we must
		if (fread(&savestate.savestatesize, sizeof(savestate.savestatesize), 1, fp) != 1) return false;
		if (savestate.savestatesize == 0) {
			Console.WriteLn(Color_StrongRed, L"[KeyMovie] The size of the savestate is invalid.");
			return false;
		}

		savestate.savestate.MakeRoomFor(savestate.savestatesize);
		// We read "savestatesize" * the size of a cell
		if (fread(savestate.savestate.GetPtr(), sizeof(savestate.savestate[0]), savestate.savestatesize, fp)
			!= savestate.savestatesize) return false;

		// We load the savestate
		memLoadingState load(savestate.savestate);
		UI_DisableSysActions();
		GetCoreThread().Pause();
		SysClearExecutionCache();
		load.FreezeAll();
		GetCoreThread().Resume();
	}
	else {
		// We restart the game
		// TODO fix this UI_DisableSysReset();
		sApp.SysExecute();
	}

	// ID
	if (header.ID != 0xCC) {
		return false;
	}
	// ver
	if (header.version != 3) {
		return false;
	}
	return true;
}
bool KeyMovieOnFile::writeHeader()
{
	if (fp == NULL)return false;
	rewind(fp);
	if (fwrite(&header, sizeof(KeyMovieHeader), 1, fp) != 1) return false;
	return true;
}
bool KeyMovieOnFile::writeSavestate()
{
	if (fp == NULL) return false;
	fseek(fp, SEEKPOINT_SAVESTATE, SEEK_SET);
	if (fwrite(&savestate.fromSavestate, sizeof(bool), 1, fp) != 1) return false;

	if (savestate.fromSavestate) {
		if (fwrite(&savestate.savestatesize, sizeof(savestate.savestatesize), 1, fp) != 1) return false;
		if (fwrite(savestate.savestate.GetPtr(), sizeof(savestate.savestate[0]), savestate.savestatesize, fp)
			!= savestate.savestatesize) return false;
	}
	return true;
}
bool KeyMovieOnFile::writeMaxFrame()
{
	if (fp == NULL)return false;
	fseek(fp, SEEKPOINT_FRAMEMAX, SEEK_SET);
	if (fwrite(&MaxFrame, 4, 1, fp) != 1) return false;
	return true;
}
void KeyMovieOnFile::updateFrameMax(unsigned long frame)
{
	if (MaxFrame >= frame) {
		return;
	}
	MaxFrame = frame;
	if (fp == NULL)return ;
	fseek(fp, SEEKPOINT_FRAMEMAX, SEEK_SET);
	fwrite(&MaxFrame, 4, 1, fp);
}
void KeyMovieOnFile::addUndoCount()
{
	UndoCount++;
	if (fp == NULL)return;
	fseek(fp, SEEKPOINT_UNDOCOUNT, SEEK_SET);
	fwrite(&UndoCount, 4, 1, fp);

}

void KeyMovieHeader::setAuthor(wxString _author)
{
	int max = ArraySize(author) - 1;
	strncpy(author, _author.c_str(), max);
	author[max] = 0;
}
void KeyMovieHeader::setCdrom(wxString _cdrom)
{
	int max = ArraySize(cdrom) - 1;
	strncpy(cdrom, _cdrom.c_str(), max);
	cdrom[max] = 0;
}
void KeyMovieHeader::init()
{
	memset(author, 0, ArraySize(author));
	memset(cdrom, 0, ArraySize(cdrom));
}

void KeyMovieOnFile::ConvertV2ToV3(wxString filename)
{
	// TODO, with the latest version of save states requiring a savestate integrated into the file
	// or it restarts as it assumes it is from power-on
	// I don't see how save states would be compatible
}


//===========================================================
// ver 1.0~1.2 -> ver 2.0~
// If the file does not have a version header, then we can assume it is prior to version....3?
// takes far too long to iterate through with the new format because of the large space reserved for
// the save state(s)
// 
// Note - Save states are not even compatible from these two versions of PCSX2
//===========================================================
void KeyMovieOnFile::ConvertV1_XToV2(wxString filename)
{
	Console.WriteLn(Color_StrongBlue, wxString::Format(L"[KeyMovie]Convert start.[%s]", WX_STR(filename)));
	FILE * fp;
	FILE * fp2;
	fopen_s(&fp, filename, "rb");
	if (fp == NULL) {
		Console.WriteLn(Color_StrongBlue, wxString::Format( L"[KeyMovie]Convert fail: %s ", WX_STR(wxString(strerror(errno)))));
		return;
	}
	wxString outfile = wxString::Format(L"%s_converted.p2m2", filename);
	fopen_s(&fp2, outfile, "wb");
	if (fp2 == NULL) {
		// TODO: add a TAS filter in the console
		// TODO: keybindings for TAS inputs
		Console.WriteLn(Color_StrongBlue, wxString::Format(L"[KeyMovie]convert fail: %s ", WX_STR(wxString(strerror(errno)))));
		fclose(fp);
		return;
	}
	//---------
	// head
	//---------
	KeyMovieHeader header;
	header.version = 2;
	u32 maxframe =0;
	u32 undo = 0;
	fread(&maxframe, 4, 1, fp);
	fread(&undo, 4, 1, fp);
	MaxFrame = maxframe;
	UndoCount = undo;
	fwrite(&header, sizeof(KeyMovieHeader), 1, fp2);
	fwrite(&MaxFrame, 4, 1, fp2);
	fwrite(&UndoCount, 4, 1, fp2);

	//---------
	// frame
	//---------
	// this routine runs forever, looks broken

	for (unsigned long i = 0; i < maxframe; i++) {
		for (u8 port = 0; port < 2; port++) {
			for (u8 buf = 3; buf < 3+6 ; buf++) {
				long seekpoint1 = i;
				seekpoint1 <<= (1 + 5);
				seekpoint1 += port;
				seekpoint1 <<= 5;
				seekpoint1 += buf;
				seekpoint1 += 8;
				long seekpoint2 = _getBlockSeekPoint(i) + BLOCK_HEADER_SIZE + 6 * port + (buf - 3);
				u8 tmp = 0;
				fseek(fp, seekpoint1, SEEK_SET);
				fread(&tmp, 1, 1, fp);
				fseek(fp2, seekpoint2, SEEK_SET);
				fwrite(&tmp, 1, 1, fp2);
			}
		}
	}
	fclose(fp);
	fclose(fp2);
	Console.WriteLn(Color_StrongBlue, wxString::Format(L"[KeyMovie]Convert success. OutFile[%s]", WX_STR(outfile)));
}

void KeyMovieOnFile::ConvertV1ToV2(wxString filename)
{
	// TODO: Save states are not compatible across these releases
	// so unable to test
}

//===========================================================
// convert p2m -> p2m2
// The p2m file is a file generated with the following URL.
// https://code.google.com/archive/p/pcsx2-rr/
// Legacy Conversion this will
//===========================================================
void KeyMovieOnFile::ConvertLegacy(wxString filename)
{
	Console.WriteLn(Color_StrongBlue, wxString::Format(L"[KeyMovie]Convert start.[%s]", WX_STR(filename)));
	FILE * fp;
	FILE * fp2;
	fopen_s(&fp, filename, "rb");
	if (fp == NULL) {
		Console.WriteLn(Color_StrongBlue, L"[KeyMovie]Convert fail: %s ", WX_STR(wxString(strerror(errno))));
		return;
	}
	wxString outfile = wxString::Format(L"%s.p2m2", filename);
	fopen_s(&fp2, outfile, "wb");
	if (fp2 == NULL) {
		Console.WriteLn(Color_StrongBlue, L"[KeyMovie]Convert fail: %s ", WX_STR(wxString(strerror(errno))));
		fclose(fp);
		return;
	}

	//--------------------------------------
	// pcsx2_rr �̓��e
	// fread(&g_Movie.FrameMax, 4, 1, g_Movie.File);
	// fread(&g_Movie.Rerecs, 4, 1, g_Movie.File);
	// fread(g_PadData[0]+2, 6, 1, g_Movie.File);
	//--------------------------------------

	//------
	//head 
	//------
	KeyMovieHeader header;
	header.version = 2;
	u32 maxframe = 0;
	u32 undo = 0;
	fread(&maxframe, 4, 1, fp);
	fread(&undo, 4, 1, fp);
	MaxFrame = maxframe;
	UndoCount = undo;
	fwrite(&header, sizeof(KeyMovieHeader), 1, fp2);
	fwrite(&MaxFrame, 4, 1, fp2);
	fwrite(&UndoCount, 4, 1, fp2);

	//------
	// frame
	//------
	for (unsigned long frame = 0; frame < maxframe; frame++)
	{
		u8 p1key[6];
		fread(p1key, 6, 1, fp);

		long seek = _getBlockSeekPoint(frame) + BLOCK_HEADER_SIZE;
		fseek(fp2, seek, SEEK_SET);
		fwrite(p1key, 6, 1, fp2);

		// 2p
		u8 p2key[6] = { 255,255,127,127,127,127 };
		fwrite(p2key, 6, 1, fp2);

	}
	fclose(fp);
	fclose(fp2);
	Console.WriteLn(Color_StrongBlue, wxString::Format(L"[KeyMovie]Convert success. OutFile[%s]", WX_STR(outfile)));
}


