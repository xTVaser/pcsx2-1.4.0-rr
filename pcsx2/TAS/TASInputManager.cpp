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
	// Normal keys
	if (bufIndex <= 1)
		buf[BufCount] = buf[BufCount] & pad.buf[port][bufIndex];
		//buf[BufCount] = ~(~buf[BufCount] | ~pad.buf[port][bufIndex]);
	/*
	// Normal keys OR analog keys
	if ((bufIndex <= 1 && pad.buf[port][bufIndex] != 255)
		|| (bufIndex > 1 && pad.buf[port][bufIndex] != 127))
		buf[BufCount] = pad.buf[port][bufIndex];
	*/
}

void TASInputManager::ToggleButton(wxString button)
{
	auto normalKeys = pad.getNormalKeys(0);
	normalKeys[button] = !normalKeys[button];
	pad.setNormalKeys(0, normalKeys);
}
