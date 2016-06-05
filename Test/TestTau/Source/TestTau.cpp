#include "Main.h"
#include <Tau/TauParser.h>

using namespace std;
using namespace kai;
using namespace kai::tau;
typedef TauTokenEnumType Tok;

struct TauTest : ::testing::Test
{
	const char *input = "namespace Foo\n\tclass Bar\n\t\tint Method()\n";

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

	TauTokenEnumType::Enum res[] = {
			Tok::Namespace, Tok::Whitespace, Tok::Ident, Tok::NewLine,
			Tok::Tab, Tok::Class, Tok::Whitespace, Tok::Ident, Tok::NewLine,
			Tok::Tab, Tok::Tab, Tok::Ident, Tok::Whitespace, Tok::Ident, Tok::OpenParan,Tok::CloseParan, Tok::NewLine };
}

TEST_F(TauTest, TestParse)
{
	Registry r;
	auto l = make_shared<TauLexer>(input, r);
	l->Process();

	auto p = make_shared<TauParser>(r);
	p->Process(l, Structure::Namespace);
	ASSERT_FALSE(p->Failed);
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

