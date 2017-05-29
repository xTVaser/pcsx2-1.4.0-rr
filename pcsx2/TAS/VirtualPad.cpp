#include "PrecompiledHeader.h"
#include "TAS/VirtualPad.h"
#include "TAS/TASInputManager.h"

enum
{
	// Normal
	ID_UP=1,
	ID_RIGHT,
	ID_LEFT,
	ID_DOWN,
	ID_SELECT,
	ID_START,
	ID_X,
	ID_CIRCLE,
	ID_SQUARE,
	ID_TRIANGLE,
	ID_L1,
	ID_L2,
	ID_L3,
	ID_R1,
	ID_R2,
	ID_R3,
	// Analog (sliders)
	ID_L_UPDOWN,
	ID_L_RIGHTLEFT,
	ID_R_UPDOWN,
	ID_R_RIGHTLEFT,
	// Analog (TextCtrl)
	ID_L_UPDOWN_TEXT,
	ID_L_RIGHTLEFT_TEXT,
	ID_R_UPDOWN_TEXT,
	ID_R_RIGHTLEFT_TEXT,
	// Reset
	ID_RESET
};

wxBEGIN_EVENT_TABLE(VirtualPad, wxFrame)
	EVT_CLOSE(VirtualPad::OnClose)
wxEND_EVENT_TABLE()

VirtualPad::VirtualPad(wxWindow * parent, int controllerPort)
	: wxFrame(parent, wxID_ANY, wxString::Format("Virtual Pad %d", controllerPort), wxDefaultPosition, wxSize(575, 500), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)),
		port(controllerPort)
{
	// Global
	wxPanel *panel = new wxPanel(this, wxID_ANY);
	int x = 10, y = 2;
	int w = 50, h = 35;
	int space = 5;

	// Left triggers
	buttons[0] = new wxToggleButton(panel, ID_L2, L"L2", wxPoint(x, y), wxSize(w, h));
	buttons[1] = new wxToggleButton(panel, ID_L1, L"L1", wxPoint(x, y + h + space), wxSize(w, h));

	// Cross Key
	x = 15;
	y = 100;
	buttons[2] = new wxToggleButton(panel, ID_UP, _("Up"), wxPoint(x + w + space, y), wxSize(w, h));
	buttons[3] = new wxToggleButton(panel, ID_DOWN, _("Down"), wxPoint(x + w + space, y + 2 * h + 2 * space), wxSize(w, h));
	buttons[4] = new wxToggleButton(panel, ID_LEFT, _("Left"), wxPoint(x, y + h + 5), wxSize(w, h));
	buttons[5] = new wxToggleButton(panel, ID_RIGHT, _("Right"), wxPoint(x + 2 * w + 2 * space, y + h + space), wxSize(w, h));

	// Right triggers
	x = 475;
	y = 2;
	buttons[6] = new wxToggleButton(panel, ID_R2, L"R2", wxPoint(x, y), wxSize(w, h));
	buttons[7] = new wxToggleButton(panel, ID_R1, L"R1", wxPoint(x, y + h + space), wxSize(w, h));

	// Action buttons
	x = 365;
	y = 100;
	buttons[8] = new wxToggleButton(panel, ID_TRIANGLE, _("Triangle"), wxPoint(x + w + space, y), wxSize(w, h));
	buttons[9] = new wxToggleButton(panel, ID_X, _("X"), wxPoint(x + w + space, y + 2 * h + 2 * space), wxSize(w, h));
	buttons[10] = new wxToggleButton(panel, ID_SQUARE, _("Square"), wxPoint(x, y + h + 5), wxSize(w, h));
	buttons[11] = new wxToggleButton(panel, ID_CIRCLE, _("Circle"), wxPoint(x + 2 * w + 2 * space, y + h + space), wxSize(w, h));

	// L3, R3
	y = 20;
	buttons[12] = new wxToggleButton(panel, ID_L3, L"L3", wxPoint(150, y), wxSize(w, h));
	buttons[13] = new wxToggleButton(panel, ID_R3, L"R3", wxPoint(350, y), wxSize(w, h));

	// Start, select
	buttons[14] = new wxToggleButton(panel, ID_SELECT, _("Select"), wxPoint(150, y + h + space), wxSize(w, h));
	buttons[15] = new wxToggleButton(panel, ID_START, _("Start"), wxPoint(350, y + h + space), wxSize(w, h));

	// Left analog
	x = 5;
	y = 200;
	w = 200;
	h = 30;
	space = 3;
	sticks[0] = new wxSlider(panel, ID_L_UPDOWN, 127, 0, 255, wxPoint(x + w + space, y), wxSize(h, w),
			wxSL_VERTICAL | wxSL_INVERSE | wxSL_LEFT);
	sticks[1] = new wxSlider(panel, ID_L_RIGHTLEFT, 127, 0, 255, wxPoint(x, y + w + space), wxSize(w, h), wxSL_HORIZONTAL);

	sticksText[0] = new wxSpinCtrl(panel, ID_L_UPDOWN_TEXT, L"127", wxPoint(x + w + space + 30, y + w/2 - 10), wxSize(55, 20),
			wxSP_ARROW_KEYS | wxALIGN_LEFT, 0, 255, 127);
	sticksText[1] = new wxSpinCtrl(panel, ID_L_RIGHTLEFT_TEXT, L"127", wxPoint(x + w/2 - 10, y + w + space + 30), wxSize(55, 20),
			wxSP_ARROW_KEYS | wxALIGN_LEFT, 0, 255, 127);

	// Right analog
	x = 275;
	sticks[2] = new wxSlider(panel, ID_R_UPDOWN, 127, 0, 255, wxPoint(x + w + space, y), wxSize(h, w),
			wxSL_VERTICAL | wxSL_INVERSE | wxSL_LEFT);
	sticks[3] = new wxSlider(panel, ID_R_RIGHTLEFT, 127, 0, 255, wxPoint(x, y + w + space), wxSize(w, h), wxSL_HORIZONTAL);

	sticksText[2] = new wxSpinCtrl(panel, ID_R_UPDOWN_TEXT, L"127", wxPoint(x + w + space + 30, y + w/2 - 10), wxSize(55, 20),
			wxSP_ARROW_KEYS | wxALIGN_LEFT, 0, 255, 127);
	sticksText[3] = new wxSpinCtrl(panel, ID_R_RIGHTLEFT_TEXT, L"127", wxPoint(x + w/2 - 10, y + w + space + 30), wxSize(55, 20),
			wxSP_ARROW_KEYS | wxALIGN_LEFT, 0, 255, 127);

	// Reset
	reset = new wxButton(panel, ID_RESET, _("Reset"), wxPoint(515, 430), wxSize(50, 35));
	Bind(wxEVT_BUTTON, &VirtualPad::OnResetButton, this, ID_RESET);

	// Handling buttons (normal keys)
	for (int i = ID_UP; i <= 16; i++)
		Bind(wxEVT_TOGGLEBUTTON, &VirtualPad::OnClick, this, i);

	// Handling TextCtrl changes (analog keys)
	for (int i = ID_L_UPDOWN_TEXT; i <= ID_R_RIGHTLEFT_TEXT; i++)
		Bind(wxEVT_SPINCTRL, &VirtualPad::OnTextCtrlChange, this, i);

	// Handling Slider changes (analog keys)
	for (int i = ID_L_UPDOWN; i <= ID_R_RIGHTLEFT; i++)
		Bind(wxEVT_SLIDER, &VirtualPad::OnSliderMove, this, i);
}

bool VirtualPad::Show(bool show)
{
	if (!wxFrame::Show(show))
		return false;
	if (show)
		g_TASInput.SetVirtualPadReading(port, true);
	return true;
}

void VirtualPad::OnClose(wxCloseEvent & event)
{
	g_TASInput.SetVirtualPadReading(port, false);
	Hide();
}

void VirtualPad::OnClick(wxCommandEvent & event)
{
	if (0 < event.GetId() && event.GetId() <= 16)
		g_TASInput.SetButtonState(port, PadDataNormalKeys[event.GetId() - ID_UP], event.IsChecked());
	else
		Console.WriteLn("Virtual Pad Error: Unknown toggle button pressed");
}

void VirtualPad::OnResetButton(wxCommandEvent & event)
{
	// Normal buttons
	for (int i = 0; i < 16; i++)
	{
		buttons[i]->SetValue(false);
		g_TASInput.SetButtonState(port, PadDataNormalKeys[i], false);
	}

	// Analog
	for (int i = 0; i < 4; i++)
	{
		sticks[i]->SetValue(127);
		sticksText[i]->SetValue(127);
		g_TASInput.UpdateAnalog(port, PadDataAnalogKeys[i], 127);
	}
}

void VirtualPad::OnTextCtrlChange(wxSpinEvent & event)
{
	if (ID_L_UPDOWN_TEXT <= event.GetId() && event.GetId() <= ID_R_RIGHTLEFT_TEXT)
	{
		int id = event.GetId() - ID_L_UPDOWN_TEXT;
		sticks[id]->SetValue(event.GetInt());
		// We inverse up and down for more confort
		if (id % 2 == 0)
			g_TASInput.UpdateAnalog(port, PadDataAnalogKeys[id], 255 - event.GetInt());
		else
			g_TASInput.UpdateAnalog(port, PadDataAnalogKeys[id], event.GetInt());
	}
	else
		Console.WriteLn("Virtual Pad Error: Unknow TextCtrl change");
}

void VirtualPad::OnSliderMove(wxCommandEvent & event)
{
	if (ID_L_UPDOWN <= event.GetId() && event.GetId() <= ID_R_RIGHTLEFT)
	{
		int id = event.GetId() - ID_L_UPDOWN;
		sticksText[id]->SetValue(event.GetInt());
		// We inverse up and down for more confort
		if (id % 2 == 0)
			g_TASInput.UpdateAnalog(port, PadDataAnalogKeys[id], 255 - event.GetInt());
		else
			g_TASInput.UpdateAnalog(port, PadDataAnalogKeys[id], event.GetInt());
	}
	else
		Console.WriteLn("Virtual Pad Error: Unknow TextCtrl change");
}
