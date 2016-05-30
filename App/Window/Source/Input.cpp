#include <iostream>
#include <KAI/Console/Console.h>
#include "Fluid/Window.h"

using namespace std;
using namespace kai;

struct FluidConsole;

shared_ptr<Console> console;
shared_ptr<FluidConsole> fluidConsole;

struct FluidConsole
{
	Value<Executor> _exec;
	Value<Stack> _data;
	Value<Stack> _context;
	Fl_Text_Buffer *_dataOutput;
	Fl_Text_Buffer *_contextOutput;
	Registry *_reg;

	static FluidConsole *self;

	FluidConsole()
	{
		KaiEnter->callback(EnterPressed, 0);

		self = this;
		_exec = console->GetExecutor();
		_data = _exec->GetDataStack();
		_context = _exec->GetContextStack();
		_reg = &console->GetRegistry();

		auto d = new Fl_Text_Buffer();
		auto c = new Fl_Text_Buffer();

		KaiOutputData->buffer(d);
		KaiOutputContext->buffer(c);
		_dataOutput = KaiOutputData->buffer();
		_contextOutput = KaiOutputContext->buffer();
	}

	static void EnterPressed(Fl_Widget *, void *)
	{
		Stack &stack = *self->_data;
		try {
			console->Process(KaiInput->value());
		}
		catch (kai::Exception::Base &e) {
			stack.Push(self->_data.GetRegistry()->New<String>(e.ToString()));
		}
		catch (std::exception &e) {
			stack.Push(self->_data.GetRegistry()->New<String>(e.what()));
		}
		catch (...) {
			stack.Push(self->_reg->New<String>("Exceptional!"));
		}

		KaiInput->value("");

		Refresh();
	}

	static void Refresh()
	{
		RefreshData();
		RefreshContext();
	}

	static void RefreshData()
	{
		RefreshView(self->_data, self->_dataOutput);
	}

	static void RefreshContext()
	{
		RefreshView(self->_context, self->_contextOutput);
	}

	static void RefreshView(Value<Stack> &stack, Fl_Text_Buffer *text)
	{
		StringStream str;
		int n = stack->Size() - 1;
		for (const auto &obj : *self->_data)
		{
			str << "[" << n-- << "] "  << obj << "\n";
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

