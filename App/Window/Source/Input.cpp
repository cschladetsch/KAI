#include <KAI/Console/Console.h>
#include "Fluid/Window.h"
#include <iostream>

using namespace std;
using namespace kai;

struct FluidConsole;

shared_ptr<kai::Console> console;
shared_ptr<FluidConsole> fluidConsole;

struct FluidConsole
{
	FluidConsole(Fl_Input *input)
	{
		input->callback(ButtonPressed);
		input->when(10);
	}

	static void ButtonPressed(Fl_Widget *input, void *ptr)
	{
		cout << ptr << *(int *)ptr << endl;
	}
};

void StartKai()
{
	cout << "Started Kai" << endl;
	console = make_shared<kai::Console>();
	fluidConsole = make_shared<FluidConsole>(KaiInput);

//	KaiEnter->
}

