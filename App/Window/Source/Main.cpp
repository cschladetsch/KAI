#include <wx/chartype.h>
#include <wx/cpp.h>
#include <wx/wx.h>
#include "Window/MainWindow.h"

class MyApp : public wxApp
{
public:
	virtual bool OnInit() override
	{
		auto *win = new MainWindow(wxT("Piro Window"));
		win->Show(true);
		return true;
	}

};

IMPLEMENT_APP(MyApp)
