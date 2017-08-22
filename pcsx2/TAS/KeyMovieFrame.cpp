#include "PrecompiledHeader.h"
#include "KeyMovieFrame.h"

enum {
	File,
	Author,
	From
};

KeyMovieFrame::KeyMovieFrame(wxWindow *parent)
	: wxDialog(parent, wxID_ANY, "Movie file", wxDefaultPosition, wxSize(500, 175), wxSTAY_ON_TOP | wxCAPTION)
{
	wxPoint p;
	wxSize s = wxDefaultSize;

	// File Picker
	p.x = 15;
	p.y = 10;
	m_fileLabel = new wxStaticText(this, wxID_ANY, _("File:"), p, wxDefaultSize, wxALIGN_RIGHT);
	p.x += 80;
	p.y = 5;
	s.x = 300;
	m_filePicker = new wxFilePickerCtrl(this, File, wxEmptyString, "File", L"p2m2 file(*.p2m2)|*.p2m2", p, s);

	// Author
	p.x = 15;
	p.y = 40;
	m_authorLabel = new wxStaticText(this, wxID_ANY, _("Author:"), p, wxDefaultSize, wxALIGN_RIGHT);
	p.x += 80;
	s.x = 150;
	m_author = new wxTextCtrl(this, Author, wxEmptyString, p, s);

	// Record From
	p.x = 15;
	p.y = 70;
	m_fromLabel = new wxStaticText(this, wxID_ANY, _("Record From:"), p, wxDefaultSize, wxALIGN_RIGHT);
	p.x += 80;
	wxArrayString choices;
	choices.Add("Power-On");
	choices.Add("Now");
	m_fromChoice = new wxChoice(this, From, p, wxDefaultSize, choices);

	p.x = 280;
	p.y = 110;
	m_ok = new wxButton(this, wxID_OK, _("OK"), p);
	p.x += 100;
	m_cancel = new wxButton(this, wxID_CANCEL, _("Cancel"), p);

	// Events
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &KeyMovieFrame::OnOK, this, wxID_OK);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &KeyMovieFrame::OnCancel, this, wxID_CANCEL);
}

void KeyMovieFrame::OnOK(wxCommandEvent &event)
{
	Console.WriteLn("OK");

	Hide();
}

void KeyMovieFrame::OnCancel(wxCommandEvent &event)
{
	Console.WriteLn("CANCEL");

	Hide();
}
