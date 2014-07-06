#include "Precompiled.h"
#include <iostream>

#include "gtest/gtest.h"
#include "Translator.h"
#include "RhoLang.h"

KAI_TRANS_BEGIN

using namespace std;

#pragma comment(lib, "gtestd.lib")

RhoLang *Process(const char *text, Parser::Structure st)
{
	RhoLang *rl = new RhoLang();
	rl->Translate(text, st);
	if (rl->Failed)
		std::cerr << rl->Error << std::endl;
	return rl;
}

RhoLang *ProcessFile(const char *text)
{
	RhoLang *rl = new RhoLang();
	rl->TranslateFile(text, Parser::ParseProgram);
	if (rl->Failed)
		std::cerr << rl->Error << std::endl;
	return rl;
}

RhoLang *Fun(const char *text)
{
	return Process(text, Parser::ParseFunction);
}

RhoLang *Exp(const char *text)
{
	return Process(text, Parser::ParseExpression);
}

RhoLang *Statement(const char *text)
{
	return Process(text, Parser::ParseStatement);
}

TEST(TestLang, TestSimple)
{
	ASSERT_EQ(Exp("1")->trans->Result(), string(" 1"));
	ASSERT_EQ(Exp("foo")->trans->Result(), string(" foo"));
	ASSERT_EQ(Exp("a+1")->trans->Result(), string(" a 1 +"));
	ASSERT_EQ(Exp("2*(3+4)")->trans->Result(), string(" 2 3 4 + mul"));
	ASSERT_EQ(Exp("2/(3+4)")->trans->Result(), string(" 2 3 4 + div"));
}

TEST(TestLang, TestMultiLine)
{
	string text = R"QQ(
foo()
{
  a=1;
  b=;
  c(;
}
)QQ";
	ASSERT_EQ(Fun(text.c_str())->Failed, true);

	string text2 = R"QQ(
foo()
{
   a=1;
   b=2*(3+4);
   c(a,b+3);
}
)QQ";
	ASSERT_FALSE(Fun(text2.c_str())->Failed);
	string text3 = "foo(){}";
	ASSERT_FALSE(Fun(text3.c_str())->Failed);
}

TEST(TestLang, Failures)
{
	EXPECT_EQ(Exp("a(;")->Failed, true);
	EXPECT_EQ(Statement("a=;")->Failed, true);
	EXPECT_EQ(Exp("a+;")->Failed, true);
	EXPECT_EQ(Exp("a.;")->Failed, true);
	EXPECT_EQ(Exp("a<;")->Failed, true);
}


TEST(TestLang, TestLang)
{
	EXPECT_EQ(Exp("1")->trans->Result(), string(" 1"));
	EXPECT_EQ(Exp("1+2")->trans->Result(), string(" 1 2 +"));
	EXPECT_EQ(Exp("a[1]")->trans->Result(), string(" a 1 []"));
}

TEST(TestLang, TestIndex)
{
	const char *text = "a[1]";
	auto tr = Process(text, Parser::ParseExpression);
	EXPECT_EQ(tr->trans->Result(), string(" a 1 []"));
}

TEST(TestLang, TestPrint)
{
	//const char *text = "'f('x,'y,'z) { a()[1]; b()().c[1+2](3,4); foo.grok(1 + bar.spam(4, 5), 2, 3 + asd).baz(1,2); }";
	//const char *text = "b()().c[1+2](3,4)";
	const char *text = "b(1,2).c[1]";
	auto tr = Process(text, Parser::ParseExpression);
	//tr->Print();
}

TEST(TestLang, TestFileLoad)
{
	auto pr = ProcessFile("Scripts/Test0.rho");
	pr->Print();
}

/*

//const char *text = "'f('x,'y,'z) { a(); b(); foo.grok(1 + bar.spam(4, 5), 2, 3 + asd); eof();}";
//const char *text = "foo(a,b) { return a + b;}";
//const char *text = "if 1 + 2 * 3 < 3 * 5 + 1 && c.b.d() <= 3 { print(1); } else { print(2); }";
//const char *text = "'a=1+2<3+4;";
//const char *text = "'a('b, 1+2*(3 + 4)) { c(); }";
//const char *text = "'a('b, 'c) { c(3*(b[1] + 4)); if 1 +2 <3 { a(); } else { b(1+2); } } ";
//const char *text = "(1+(1+2))";
//const char *text = "(1*(1+2))";

*/

KAI_TRANS_END

