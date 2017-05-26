#include "PrecompiledHeader.h"
#include "TAS/VirtualPad.h"
#include "TAS/TASInputManager.h"

enum
{
	// Normal
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
	ID_START,
	ID_SELECT,
	// Analog
	ID_L_UPDOWN,
	ID_L_RIGHTLEFT,
	ID_L_UPDOWN_TEXT,
	ID_L_RIGHTLEFT_TEXT,
	ID_R_UPDOWN,
	ID_R_RIGHTLEFT,
	ID_R_UPDOWN_TEXT,
	ID_R_RIGHTLEFT_TEXT,
};

wxBEGIN_EVENT_TABLE(VirtualPad, wxFrame)
	EVT_CLOSE(VirtualPad::OnClose)
wxEND_EVENT_TABLE()

VirtualPad::VirtualPad(wxWindow * parent)
	: wxFrame(parent, wxID_ANY, L"Virtual Pad")
{
	// Global
	wxPanel *panel = new wxPanel(this, wxID_ANY);
	int x = 2, y = 2;
	int w = 50, h = 35;
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

	// Right triggers
	x = 400;
	y = 2;
	r2 = new wxToggleButton(panel, ID_R2, L"R2", wxPoint(x, y), wxSize(w, h));
	r1 = new wxToggleButton(panel, ID_R1, L"R1", wxPoint(x, y + h + space), wxSize(w, h));

	// Action buttons
	x = 300;
	y = 100;
	triangle = new wxToggleButton(panel, ID_TRIANGLE, _("Triangle"), wxPoint(x + w + space, y), wxSize(w, h));
	xButton = new wxToggleButton(panel, ID_X, _("X"), wxPoint(x + w + space, y + 2 * h + 2 * space), wxSize(w, h));
	square = new wxToggleButton(panel, ID_SQUARE, _("Square"), wxPoint(x, y + h + 5), wxSize(w, h));
	circle = new wxToggleButton(panel, ID_CIRCLE, _("Circle"), wxPoint(x + 2 * w + 2 * space, y + h + space), wxSize(w, h));

	// L3, R3
	y = 20;
	l3 = new wxToggleButton(panel, ID_L3, L"L3", wxPoint(150, y), wxSize(w, h));
	r3 = new wxToggleButton(panel, ID_R3, L"R3", wxPoint(250, y), wxSize(w, h));

	// Start, select
	select = new wxToggleButton(panel, ID_SELECT, _("Select"), wxPoint(150, y + h + space), wxSize(w, h));
	start = new wxToggleButton(panel, ID_START, _("Start"), wxPoint(250, y + h + space), wxSize(w, h));

	// Left analog
	x = 5;
	y = 200;
	w = 200;
	h = 30;
	space = 3;
	l_upDown = new wxSlider(panel, ID_L_UPDOWN, 127, 0, 255, wxPoint(x + w + space, y), wxSize(h, w), wxSL_VERTICAL | wxSL_INVERSE | wxSL_LEFT);
	l_rightLeft = new wxSlider(panel, ID_L_RIGHTLEFT, 127, 0, 255, wxPoint(x, y + w + space), wxSize(w, h), wxSL_HORIZONTAL);

	l_upDownText = new wxSpinCtrl(panel, ID_L_UPDOWN_TEXT, L"127", wxPoint(x + w + space + 30, y + w/2 - 10), wxSize(55, 20), wxSP_ARROW_KEYS | wxALIGN_LEFT, 0, 255, 127);
	l_rightLeftText = new wxSpinCtrl(panel, ID_L_RIGHTLEFT_TEXT, L"127", wxPoint(x + w/2 - 10, y + w + space + 30), wxSize(55, 20), wxSP_ARROW_KEYS | wxALIGN_LEFT, 0, 255, 127);

	for (int i = ID_UP; i <= ID_SELECT; i++)
		Bind(wxEVT_TOGGLEBUTTON, &VirtualPad::OnClick, this, i);
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
	case ID_DOWN:
		g_TASInput.ToggleButton("down");
		break;
	case ID_LEFT:
		g_TASInput.ToggleButton("left");
		break;
	case ID_RIGHT:
		g_TASInput.ToggleButton("right");
		break;
	case ID_L1:
		g_TASInput.ToggleButton("l1");
		break;
	case ID_L2:
		g_TASInput.ToggleButton("l2");
		break;
	case ID_L3:
		g_TASInput.ToggleButton("l3");
		break;
	case ID_R1:
		g_TASInput.ToggleButton("r1");
		break;
	case ID_R2:
		g_TASInput.ToggleButton("r2");
		break;
	case ID_R3:
		g_TASInput.ToggleButton("r3");
		break;
	case ID_SQUARE:
		g_TASInput.ToggleButton("square");
		break;
	case ID_X:
		g_TASInput.ToggleButton("x");
		break;
	case ID_TRIANGLE:
		g_TASInput.ToggleButton("triangle");
		break;
	case ID_CIRCLE:
		g_TASInput.ToggleButton("circle");
		break;
	case ID_START:
		g_TASInput.ToggleButton("start");
		break;
	case ID_SELECT:
		g_TASInput.ToggleButton("select");
		break;
	default:
		Console.WriteLn("Virtual Pad Error: Unknown toggle button pressed");
		break;
	}
}
