#include "KAI/KAI.h"
//#include "Precompiled.h"
//#include <iostream>
//
//#include "gtest/gtest.h"
//#include "Translator.h"
//#include "RhoLang.h"
//
//#include "KAI/Executor.h"
//
//KAI_BEGIN
//
//using namespace std;
//
//#pragma comment(lib, "gtestd.lib")
//
//RhoLang *Process(const char *text, Parser::Structure st, bool print = false)
//{
//	RhoLang *rl = new RhoLang();
//	rl->Translate(text, st);
//	if (print)
//		rl->Print();
//	if (rl->Failed)
//		std::cerr << rl->Error << std::endl;
//	return rl;
//}
//
//RhoLang *ProcessFile(const char *text)
//{
//	RhoLang *rl = new RhoLang();
//	rl->TranslateFile(text, Parser::ParseProgram);
//	if (rl->Failed)
//		std::cerr << rl->Error << std::endl;
//	return rl;
//}
//
//RhoLang *Fun(const char *text)
//{
//	return Process(text, Parser::ParseFunction);
//}
//
//RhoLang *Exp(const char *text, bool print = false)
//{
//	return Process(text, Parser::ParseExpression, print);
//}
//
//RhoLang *Statement(const char *text)
//{
//	return Process(text, Parser::ParseStatement);
//}
//
//TEST(TestLang, TestSimple)
//{
//	ASSERT_EQ(Exp("1")->trans->Result(), string(" 1"));
//	ASSERT_EQ(Exp("foo")->trans->Result(), string(" foo"));
//	ASSERT_EQ(Exp("a+1")->trans->Result(), string(" a 1 Plus"));
//	ASSERT_EQ(Exp("2*(3+4)")->trans->Result(), string(" 2 3 4 Plus Multiply"));
//	ASSERT_EQ(Exp("2/(3+4)")->trans->Result(), string(" 2 3 4 Plus Divide"));
//
//	ASSERT_EQ(Exp("a && b || (2*(3+4) < 4)")->trans->Result(), string(" a b LogicalAnd 2 3 4 Plus Multiply 4 Less LogicalOr"));
//}
//
//TEST(TestLang, TestStringLiteral)
//{
//	string s0 = "\"\"";
//	string s1 = "\"a\"";
//	string s2 = "\"this is \\\"a string\\\"\"";
//
//	ASSERT_EQ(Exp(s0.c_str())->trans->Result(), " " + s0);
//	ASSERT_EQ(Exp(s1.c_str())->trans->Result(), " " + s1);
//	ASSERT_EQ(Exp(s2.c_str())->trans->Result(), " " + s2);
//}
//
//TEST(TestLang, TestMultiLine)
//{
//	string text = R"QQ(
//foo()
//{
//  a=1;
//  b=;
//  c(;
//}
//)QQ";
//	ASSERT_EQ(Fun(text.c_str())->Failed, true);
//
//	string text2 = R"QQ(
//foo()
//{
//   a=1;
//   b=2*(3+4);
//   c(a,b+3);
//}
//)QQ";
//	ASSERT_FALSE(Fun(text2.c_str())->Failed);
//	string text3 = "foo(){}";
//	ASSERT_FALSE(Fun(text3.c_str())->Failed);
//}
//
//TEST(TestLang, Failures)
//{
//	EXPECT_EQ(Exp("a(;")->Failed, true);
//	EXPECT_EQ(Statement("a=;")->Failed, true);
//	EXPECT_EQ(Exp("a+;")->Failed, true);
//	EXPECT_EQ(Exp("a.;")->Failed, true);
//	EXPECT_EQ(Exp("a<;")->Failed, true);
//}
//
//
//TEST(TestLang, TestPrint)
//{
//	//const char *text = "'f('x,'y,'z) { a()[1]; b()().c[1+2](3,4); foo.grok(1 + bar.spam(4, 5), 2, 3 + asd).baz(1,2); }";
//	//const char *text = "b()().c[1+2](3,4)";
//	const char *text = "b(1,2).c[1]";
//	auto tr = Process(text, Parser::ParseExpression);
//	EXPECT_STREQ(tr->trans->Result().c_str(), " 1 2 b Suspend c GetProperty 1 Index");
//}
//
//TEST(TestLang, TestFileLoad)
//{
//	auto pr = ProcessFile("Scripts/Test0.rho");
//	cout << pr->trans->Result();
//}
//
//TEST(TestLang, TestExec)
//{
//	auto p = new RhoLang();
//	p->Exec("1+2*(3+4)");
//	auto result = p->exec->GetDataStack()->Top();
//	EXPECT_EQ(ConstDeref<int>(result), 15);
//}
//
///*
//
////const char *text = "'f('x,'y,'z) { a(); b(); foo.grok(1 + bar.spam(4, 5), 2, 3 + asd); eof();}";
////const char *text = "foo(a,b) { return a + b;}";
////const char *text = "if 1 + 2 * 3 < 3 * 5 + 1 && c.b.d() <= 3 { print(1); } else { print(2); }";
////const char *text = "'a=1+2<3+4;";
////const char *text = "'a('b, 1+2*(3 + 4)) { c(); }";
////const char *text = "'a('b, 'c) { c(3*(b[1] + 4)); if 1 +2 <3 { a(); } else { b(1+2); } } ";
////const char *text = "(1+(1+2))";
////const char *text = "(1*(1+2))";
//
//*/
//
//KAI_END
//
