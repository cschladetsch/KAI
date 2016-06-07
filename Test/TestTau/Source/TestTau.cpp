#include "Main.h"
#include <strstream>
#include <iostream>
#include <Tau/TauParser.h>
#include <Generate/Proxy.h>
#include <Generate/Agent.h>

using namespace std;
using namespace kai;
using namespace kai::tau;
typedef TauTokenEnumType Tok;

struct TauTest : ::testing::Test
{
	const char *input =
		"namespace Foo { class Bar { int Method(string s, float c); string name; } }";

	virtual void SetUp() override
	{
	}

	virtual void TearDown() override
	{
	}
};

TEST_F(TauTest, TestLex1)
{
	Registry r;
	TauLexer l(input, r);
	l.Process();
	if (l.Failed)
		cerr << l.Error << endl;
//
//	TauTokenEnumType::Enum res[] = {
//			Tok::Namespace, Tok::Whitespace, Tok::Ident, Tok::NewLine,
//			Tok::Tab, Tok::Class, Tok::Whitespace, Tok::Ident, Tok::NewLine,
//			Tok::Tab, Tok::Tab, Tok::Ident, Tok::Whitespace, Tok::Ident, Tok::OpenParan,Tok::CloseParan, Tok::NewLine };
}

TEST_F(TauTest, TestParse)
{
	try
	{
		Registry r;
		auto l = make_shared<TauLexer>(input, r);
		l->Process();
		if (l->Failed)
			cerr << l->Error << endl;
		ASSERT_FALSE(l->Failed);
		cout << l->Print() << endl;

		auto p = make_shared<TauParser>(r);
		p->Process(l, Structure::Namespace);
		if (p->Failed)
			cerr << p->Error << endl;
		ASSERT_FALSE(p->Failed);
		cout << p->PrintTree();

		Generate::Proxy g;
		EXPECT_TRUE(g.Generate(*p, "TestParse.proxy.h"));

		Generate::Agent a;
		EXPECT_TRUE(a.Generate(*p, "TestParse.agent.h"));
	}
	catch (Exception::Base &e)
	{
		cerr << e.ToString() << endl;
		FAIL();
	}
}

TEST_F(TauTest, TestGenerate)
{
	Registry r;
	auto l = make_shared<TauLexer>(input, r);
	l->Process();

	auto p = make_shared<TauParser>(r);
	p->Process(l, Structure::Namespace);
	ASSERT_FALSE(p->Failed);

//	auto t = new TauTranslator(r);
//	t->GenerateInterface("TestInterface.h", *p);
//	t->GenerateInterface("TestProxy.h", *p);
//	t->GenerateInterface("TestAgent.h", *p);
}

