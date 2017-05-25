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

	/*
	int bufIndex = BufCount - 3;
	if (0 < bufIndex || bufIndex < 6)
		return;
	Console.WriteLn("KOJOJ");
	buf[BufCount] = pad.buf[port][bufIndex];
	*/
}

void TASInputManager::ToggleButton(wxString button)
{
	auto normalKeys = pad.getNormalKeys(0);
	normalKeys[button] = !normalKeys[button];
	pad.setNormalKeys(0, normalKeys);
}
