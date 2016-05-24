#include "Main.h"
#include "Button.h"
#include "Communicate.h"
#include <memory>
#include <wx/wx.h>
#include <wx/string.h>

using namespace std;

class Simple : public wxFrame
{
public:
	Simple(const wxString& title)
		: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 150))
	{
		Center();
	}

};

class MyApp : public wxApp
{
public:
	virtual bool OnInit() override
	{
		Communicate *communicate = new Communicate(wxT("Widgets communicate"));
		communicate->Show(true);
		return true;
	}

};

IMPLEMENT_APP(MyApp)

