#pragma once

#include "Window/TextEntry.h"
#include "Window/ExecutorState.h"

struct MainWindow : wxFrame
{
	MainWindow(const wxString& title);

private:
	wxPanel *m_parent;
	std::shared_ptr<TextEntry> _textEntry;
	std::shared_ptr<ExecutorState> _execState;
};

