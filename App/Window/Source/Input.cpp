#include <KAI/Console/Console.h>
#include <KAI/Core/Exception.h>
#include <KAI/Core/BuiltinTypes.h>

#include "Fluid/Window.h"

#include <iostream>

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
		KaiInput->callback(ButtonPressed, 0);
		KaiEnter->callback(EnterPressed, 0);

		self = this;
		_exec = console->GetExecutor();
		_data = _exec->GetDataStack();
		_context = _exec->GetContextStack();
		_reg =& console->GetRegistry();

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
		try
		{
			console->Process(KaiInput->value());
		}
		catch (kai::Exception::Base &e)
		{
			stack.Push(self->_data.GetRegistry()->New<String>(e.ToString()));
		}
		catch (std::exception& e)
		{
			stack.Push(self->_data.GetRegistry()->New<String>(e.what()));
		}
		catch (...)
		{
			stack.Push(self->_reg->New<String>("Exceptional!"));
		}

		KaiInput->value("");

		Refresh();
	}

	static void Refresh()
	{
		RefreshDataView();
		RefreshContextView();
	}

	static void RefreshContextView()
	{
	}

	static void RefreshDataView()
	{
		StringStream str;
		int n = self->_data->Size() - 1;
		for (const auto &obj : *self->_data)
		{
			str << "[" << n-- << "] "  << obj << "\n";
		}

		self->_dataOutput->text(str.ToString().c_str());
	}

	static void ButtonPressed(Fl_Widget *input, void *ptr)
	{
		//cout << "Button pressed" << endl;
	}
};

FluidConsole *FluidConsole::self;

void StartKai()
{
	cout << "Started Kai" << endl;
	console = make_shared<kai::Console>();
	console->SetLanguage(Language::Pi);
	fluidConsole = make_shared<FluidConsole>();
}

