#pragma once
#include <wx/wx.h>
#include <wx/filepicker.h>

/*
 * The Dialog to pop-up when recording a new movie
*/
class KeyMovieFrame : public wxDialog
{
public:
	KeyMovieFrame(wxWindow *parent);
	
private:
	wxStaticText *m_fileLabel;
	wxFilePickerCtrl *m_filePicker;
	wxStaticText *m_authorLabel;
	wxTextCtrl *m_author;
	wxStaticText *m_fromLabel;
	wxChoice *m_fromChoice;
	wxButton *m_ok;
	wxButton *m_cancel;

private:
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
};

