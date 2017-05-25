#pragma once
#include "TAS/PadData.h"

class TASInputManager
{
public:
	TASInputManager();

	void ControllerInterrupt(u8 &data, u8 &port, u16 & BufCount, u8 buf[]);

	/*
	 Toggles the normal buttons
	*/
	void ToggleButton(wxString button);

protected:
	PadData pad;
};
extern TASInputManager g_TASInput;

