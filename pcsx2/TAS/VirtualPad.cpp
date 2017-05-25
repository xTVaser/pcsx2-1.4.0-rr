#include "PrecompiledHeader.h"
#include "TAS/VirtualPad.h"
#include "TAS/TASInputManager.h"

enum
{
	ID_UP=1,
	ID_DOWN,
	ID_LEFT,
	ID_RIGHT,
	ID_L1,
	ID_L2,
	ID_R1,
	ID_R2,
	ID_L3,
	ID_R3,
	ID_SQUARE,
	ID_X,
	ID_CIRCLE,
	ID_TRIANGLE,
};

wxBEGIN_EVENT_TABLE(VirtualPad, wxFrame)
	EVT_TOGGLEBUTTON(wxID_ANY, VirtualPad::OnClick)
	EVT_CLOSE(VirtualPad::OnClose)
wxEND_EVENT_TABLE()

VirtualPad::VirtualPad(wxWindow * parent)
	: wxFrame(parent, wxID_ANY, L"Virtual Pad")
{
	// Global
	wxPanel *panel = new wxPanel(this, wxID_ANY);
	int x = 2, y = 2;
	int w = 40, h = 35;
	int space = 5;

	// Left triggers
	l2 = new wxToggleButton(panel, ID_L2, L"L2", wxPoint(x, y), wxSize(w, h));
	l1 = new wxToggleButton(panel, ID_L1, L"L1", wxPoint(x, y + h + space), wxSize(w, h));

	// Cross Key
	x = 2;
	y = 100;
	up = new wxToggleButton(panel, ID_UP, _("Up"), wxPoint(x + w + space, y), wxSize(w, h));
	down = new wxToggleButton(panel, ID_DOWN, _("Down"), wxPoint(x + w + space, y + 2 * h + 2 * space), wxSize(w, h));
	left = new wxToggleButton(panel, ID_LEFT, _("Left"), wxPoint(x, y + h + 5), wxSize(w, h));
	right = new wxToggleButton(panel, ID_RIGHT, _("Right"), wxPoint(x + 2 * w + 2 * space, y + h + space), wxSize(w, h));
}

void VirtualPad::UpdateInputs() const
{
	PadData pad;
	auto normalKeys = pad.getNormalKeys(0);

	if (up->GetValue())
		normalKeys.at("up") = true;

	pad.setNormalKeys(0, normalKeys);
}

void VirtualPad::OnClose(wxCloseEvent & event)
{
	Hide();
}

void VirtualPad::OnClick(wxCommandEvent & event)
{
	switch (event.GetId())
	{
	case ID_UP:
		g_TASInput.ToggleButton("up");
		break;
	default:
		Console.WriteLn("Shouldn't occur");
		break;
	}
}
