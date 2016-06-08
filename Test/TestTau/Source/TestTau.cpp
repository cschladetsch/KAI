#include "Main.h"
#include <KAI/Language/Tau/TauParser.h>
#include <KAI/Network/Proxy.h>
#include <KAI/Language/Tau/Generate/Agent.h>
#include <KAI/Language/Tau/Generate/Proxy.h>

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
//		cout << l->Print() << endl;

		auto p = make_shared<TauParser>(r);
		p->Process(l, Structure::Namespace);
		if (p->Failed)
			cerr << p->Error << endl;
		ASSERT_FALSE(p->Failed);
//		cout << p->PrintTree();

		Generate::Proxy g;
		EXPECT_TRUE(g.Generate(*p, "TestParse.proxy.h"));
		if (g.Failed)
			cerr << g.Error << endl;

//		Generate::Agent a;
//		EXPECT_TRUE(a.Generate(*p, "TestParse.agent.h"));
//		if (a.Failed)
//			cerr << a.Error << endl;
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
}

