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
	void SetButtonState(wxString button, bool state);

	// Handles analog sticks
	void UpdateAnalog(wxString key, int value);

	void SetVirtualPadReading(bool read=true);

protected:
	PadData pad;
	bool virtualPad;
};
extern TASInputManager g_TASInput;

