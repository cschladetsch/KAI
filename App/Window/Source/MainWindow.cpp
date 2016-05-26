#include "Window/MainWindow.h"

using namespace std;

MainWindow::MainWindow(const wxString &title)
{
	auto box = new wxBoxSizer(wxVERTICAL);

	_textEntry = make_shared<TextEntry>(this);
	_execState = make_shared<ExecutorState>(this);

	box->Add(&*_textEntry, 1, wxEXPAND | wxALL, 5);
	box->Add(&*_execState, 1, wxEXPAND | wxALL, 5);

	m_parent->SetSizer(box);
}
