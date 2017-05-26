#pragma once
#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <wx/spinctrl.h>

#include "TAS/PadData.h"

class VirtualPad : public wxFrame
{
public:
	VirtualPad(wxWindow *parent);

	void UpdateInputs() const;

protected:
	wxToggleButton *up, *down, *left, *right;
	wxToggleButton *r1, *r2, *r3;
	wxToggleButton *l1, *l2, *l3;
	wxToggleButton *square, *circle, *xButton, *triangle;
	wxToggleButton *select, *start;

	wxSlider *l_upDown, *l_rightLeft;

	wxSpinCtrl *l_upDownText, *l_rightLeftText;

protected:
	void OnClose(wxCloseEvent &event);

	void OnClick(wxCommandEvent &event);

	wxDECLARE_EVENT_TABLE();
};

