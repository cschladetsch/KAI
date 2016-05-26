#pragma once

#include <wx/panel.h>
#include <KAI/Console/Console.h>

/// A window showing the state of an executor
struct ExecutorState : wxPanel
{
	ExecutorState(wxWindow *parent);

	kai::Pointer<kai::Executor> GetExecutor() const;
	kai::Registry &GetRegistry() const;

private:
	kai::Pointer<kai::Console> _exec;
};

