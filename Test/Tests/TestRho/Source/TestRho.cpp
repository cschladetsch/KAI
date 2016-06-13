#include "Main.h"

using namespace kai;
using namespace std;

TEST(TestRho, TestPaths)
{
	Registry r;
	auto paths = vector<string> {"foo/bar", "'foo", "/foo", "foo", "'/foo/bar/spam"};
	for (auto p : paths)
	{
		auto lex = make_shared<RhoLexer>(p.c_str(), r);
		ASSERT_TRUE(lex->Process());
		cout << p << endl;
		cout << lex->Print() << endl;
		auto parse = make_shared<RhoParser>(r);
		ASSERT_TRUE(parse->Process(lex, Structure::Expression));
		cout << parse->PrintTree() << endl;
	}
}

TEST(TestRho, TestAssignment)
{
	try
	{
		TranslatorCommon::trace = 2;

		Console console;
		console.SetLanguage(Language::Rho);
		console.Execute("1;");
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

