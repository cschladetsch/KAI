#include <iostream>
#include <KAI/Console/Console.h>
#include "Fluid/Window.h"

using namespace std;
using namespace kai;

struct FluidConsole;

shared_ptr<Console> console;
shared_ptr<FluidConsole> fluidConsole;

/// The console for KAI provided bu Fluid
struct FluidConsole
{
	Registry *_reg;
	Value<Executor> _exec;
	Value<Stack> _data;
	Value<Stack> _context;

	Fl_Text_Buffer *_dataOutput;
	Fl_Text_Buffer *_contextOutput;

	static FluidConsole *self;

	FluidConsole()
	{
		KaiEnter->callback(EnterPressed, 0);

		self = this;
		_exec = console->GetExecutor();
		_data = _exec->GetDataStack();
		_context = _exec->GetContextStack();
		_reg = &console->GetRegistry();

		_dataOutput = new Fl_Text_Buffer();
		_contextOutput = new Fl_Text_Buffer();

		KaiOutputData->buffer(_dataOutput);
		KaiOutputContext->buffer(_contextOutput);
	}

	static void EnterPressed(Fl_Widget *, void *)
	{
		Stack &stack = *self->_data;
		auto &reg = *self->_reg;

		try
		{
			console->Process(KaiInput->value());
		}
		catch (kai::Exception::Base &e)
		{
			stack.Push(reg.New<String>(e.ToString()));
		}
		catch (std::exception &e)
		{
			stack.Push(reg.New<String>(e.what()));
		}
		catch (...)
		{
			stack.Push(reg.New<String>("Exceptional!"));
		}

		KaiInput->value("");

		Refresh();
	}

	static void Refresh()
	{
		RefreshView(self->_data, self->_dataOutput);
		RefreshView(self->_context, self->_contextOutput);
	}

	static void RefreshView(Value<Stack> &stack, Fl_Text_Buffer *text)
	{
		StringStream str;
		int n = 0;
		auto const &st = stack->GetStack();
		auto end = st.rend();
		auto begin = st.rbegin();
		for (; end != begin; ++begin)
		{
			str << "[" << n++ << "] "  << *begin << "\n";
		}

		text->text(str.ToString().c_str());
	}
};

FluidConsole *FluidConsole::self;

void StartKai()
{
	cout << "Starting Kai" << endl;
	console = make_shared<kai::Console>();
	console->SetLanguage(Language::Pi);
	fluidConsole = make_shared<FluidConsole>();
}
