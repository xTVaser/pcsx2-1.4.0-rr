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

	void UpdateAnalog(wxString key, int value);

protected:
	PadData pad;
};
extern TASInputManager g_TASInput;

