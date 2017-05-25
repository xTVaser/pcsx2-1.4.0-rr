#include "PrecompiledHeader.h"
#include "TASInputManager.h"
#include "lua/LuaManager.h"

TASInputManager g_TASInput;

TASInputManager::TASInputManager()
{
}

void TASInputManager::ControllerInterrupt(u8 & data, u8 & port, u16 & BufCount, u8 buf[])
{
	g_Lua.ControllerInterrupt(data, port, BufCount, buf);
}

void TASInputManager::ToggleButton(wxString button)
{
	auto normalKeys = pad.getNormalKeys(0);
	normalKeys[button] = !normalKeys[button];
	pad.setNormalKeys(0, normalKeys);
}
