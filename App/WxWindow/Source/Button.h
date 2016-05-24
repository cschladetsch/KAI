#pragma once

#include "Main.h"

class Button : public wxFrame
{
public:
	Button(const wxString& title);

	void OnQuit(wxCommandEvent & event);
};

