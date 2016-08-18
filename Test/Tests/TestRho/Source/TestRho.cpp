#include "Main.h"

using namespace kai;
using namespace std;

TEST(TestRho, TestAssignment)
{
	try
	{
//		TranslatorCommon::trace = 2;

		Console console;
		console.SetLanguage(Language::Rho);
		console.Execute("'/a=1+2;");

		Object root = console.GetRoot();
		auto a = root.GetChild(Label("a"));
		ASSERT_EQ(Deref<int>(a), 3);
	}
	catch (Exception::Base &e)
	{
		cerr << e.ToString() << endl;
	}
	catch (std::exception &e)
	{
		cerr << e.what() << endl;
	}
	catch (...)
	{
		cerr << "Exception" << endl;
	}
}

TEST(TestRho, TestExecFile)
{
	Console c;
	c.ExecuteFile("TestExecFile.rho");
	ASSERT_EQ(ConstDeref<int>(c.GetRoot().Get(Label("a"))), 123);
}

TEST(TestRho, TestFib)
{
	Console c;
	c.ExecuteFile("TestFib.rho");
}