#include "Main.h"
#include <strstream>
#include <iostream>
#include <Tau/TauParser.h>

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

struct ProxyGenerator : Process
{
	stringstream _str;
	int _indentation = 0;

	bool Generate(TauParser const &p, const char *fname)
	{
		TauParser::AstNodePtr root = p.GetRoot();
		switch (root->GetType())
		{
			case TauAstEnumType::Namespace:
			{
				if (!GenerateNameSpace(*root))
					return false;
				fstream f(fname);
				const string &s = _str.str();
				return f.write(s.c_str(), s.size()).good();
			}
		}

		return Fail("Namespace expected");
	}

	string EndLine()
	{
		_str << "\n";
		return move(string(_indentation, '\t'));
	}

	bool GenerateNameSpace(TauParser::AstNode const &ns)
	{
		_str << "namespace " << ns.GetToken().Text() << EndLine() << '{';
		_indentation++;
		for (auto const &ch : ns.GetChildren())
		{
			switch (ch->GetType())
			{
			case TauAstEnumType::Class:
				return GenerateClass(*ch);
			}
		}
		_indentation--;
		return false;
	}

	bool GenerateClass(TauParser::AstNode const &cl)
	{
		_str << "class " << cl.GetToken().Text() << EndLine() << '{';
		_indentation++;
		for (const auto &member : cl.GetChildren())
		{
			switch (member->GetType())
			{
			case TauAstEnumType::Property:
				if (!GenerateProperty(*member))
					return false;

			case TauAstEnumType::Method:
				if (!GenerateMethod(*member))
					return false;

			default:
				return Fail("Unknown class member: %s"
					, TauAstEnumType::ToString(member->GetType()));
			}
		}

		_indentation--;
		_str << '}' << EndLine();
	}

	bool GenerateProperty(TauParser::AstNode const &prop)
	{
		_str
			<< "IFuture<" << prop.GetChild(0)->GetTokenText() << "> "
			<< prop.GetChild(1)->GetTokenText() << ';' << EndLine();
		return true;
	}

	bool GenerateMethod(TauParser::AstNode const &method)
	{
		auto const &rt = method.GetChild(0);
		auto const &name = method.GetChild(1);
		auto const &args = method.GetChild(2);
//		auto const &konst = method.GetChild(3);

		_str << "IFuture<" << rt->GetTokenText() << "> " << name->GetTokenText() << "(";
		bool first = true;
		for (auto const &a : args->GetChildren())
		{
			if (!first)
				_str << ", ";

			auto &ty = a->GetChild(0);
			auto &id = a->GetChild(1);
			_str << ty->GetTokenText() << " " << id->GetTokenText();

			first = false;
		}

		_str << ';' << EndLine();
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
		cout << l->Print();

		auto p = make_shared<TauParser>(r);
		p->Process(l, Structure::Namespace);
		if (p->Failed)
			cerr << p->Error << endl;
		cout << p->PrintTree();
		ASSERT_FALSE(p->Failed);

		ProxyGenerator g;
		EXPECT_TRUE(g.Generate(*p, "TestParse.int"));
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

