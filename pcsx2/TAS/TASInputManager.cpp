#include "PrecompiledHeader.h"
#include "TASInputManager.h"
#include "lua/LuaManager.h"

TASInputManager g_TASInput;

TASInputManager::TASInputManager()
{
}

void TASInputManager::ControllerInterrupt(u8 & data, u8 & port, u16 & BufCount, u8 buf[])
{
	if (port >= 2)
		return;

	g_Lua.ControllerInterrupt(data, port, BufCount, buf);

	int bufIndex = BufCount - 3;
	if (bufIndex < 0 || 6 < bufIndex)
		return;
	Console.WriteLn(wxString::Format("%d", pad.buf[port][bufIndex]));
	// Normal keys
	if (bufIndex <= 1)
		buf[BufCount] = buf[BufCount] & pad.buf[port][bufIndex];
	// Analog keys
	else if (pad.buf[port][bufIndex] != 127)
		buf[BufCount] = pad.buf[port][bufIndex];
	
}

void TASInputManager::ToggleButton(wxString button)
{
	auto normalKeys = pad.getNormalKeys(0);
	normalKeys.at(button) = !normalKeys[button];
	pad.setNormalKeys(0, normalKeys);
}

void TASInputManager::UpdateAnalog(wxString key, int value)
{
	Console.WriteLn(wxString::Format("%s %d", key, value));
	auto analogKeys = pad.getAnalogKeys(0);
	analogKeys.at(key) = value;
	pad.setAnalogKeys(0, analogKeys);
	Console.WriteLn(wxString::Format("%d %d %d %d", pad.buf[0][2], pad.buf[0][3], pad.buf[0][4], pad.buf[0][5]));
}
