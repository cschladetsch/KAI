#include "TestBase.h"

using namespace kai;
using namespace std;

namespace
{
	TEST(PiParser, TestSimple)
	{
		auto input = "1";

		shared_ptr<PiLexer> lex = make_shared<PiLexer>(input);
		lex->Process();
		KAI_TRACE_1(lex->Print());

		shared_ptr<PiParser> parse = make_shared<PiParser>(lex);
		parse->Run(Structure::Sequence);

		KAI_TRACE_1(parse->Print());
		ASSERT_FALSE(lex->Failed);
		ASSERT_FALSE(parse->Failed);

		auto root = parse->GetRoot();
		ASSERT_TRUE(root);
		ASSERT_EQ(root->Children.size(), 2);
		ASSERT_EQ(root->Children[0]->token.type, PiTokens::Int);
		ASSERT_EQ(root->Children[1]->token.type, PiTokens::NewLine);
	}

	TEST(PiParser, TestCompoundWorks)
	{
		auto input = "[[[]]]";
		shared_ptr<PiLexer> lex = make_shared<PiLexer>(input);
		lex->Process();
		KAI_TRACE_1(lex->Print());
		if (lex->Failed)
			KAI_TRACE_1(lex->Error);

		shared_ptr<PiParser> parse = make_shared<PiParser>(lex);
		parse->Run(Structure::Sequence);

		KAI_TRACE_1(parse->Print());
		ASSERT_FALSE(lex->Failed);
		ASSERT_FALSE(parse->Failed);

		auto root = parse->GetRoot();
		ASSERT_EQ(root->Children.size(), 2);

		auto ch0 = root->Children[0];
		auto ch1 = ch0->Children[0];
		auto ch2 = ch1->Children[0];

		ASSERT_EQ(ch0->type, PiAstNodes::Array);
		ASSERT_EQ(ch1->type, PiAstNodes::Array);
		ASSERT_EQ(ch2->type, PiAstNodes::Array);
	}

	TEST(PiParser, TestMixedCompound)
	{
		auto input = "{ [ 1 2 3 ] 'a # }";

		shared_ptr<PiLexer> lex = make_shared<PiLexer>(input);
		lex->Process();
		KAI_TRACE_1(lex->Print());
		if (lex->Failed)
			KAI_TRACE_1(lex->Error);

		shared_ptr<PiParser> parse = make_shared<PiParser>(lex);
		parse->Run(Structure::Sequence);

		KAI_TRACE_1(parse->Print());
		ASSERT_FALSE(lex->Failed);
		ASSERT_FALSE(parse->Failed);
	}

	TEST(PiParser, TestCompoundFail)
	{
		auto input = "[";
		shared_ptr<PiLexer> lex = make_shared<PiLexer>(input);
		lex->Process();
		KAI_TRACE_1(lex->Print());
		if (lex->Failed)
			KAI_TRACE_1(lex->Error);

		shared_ptr<PiParser> parse = make_shared<PiParser>(lex);
		parse->Run(Structure::Sequence);

		KAI_TRACE_1(parse->Print());
		ASSERT_FALSE(lex->Failed);
		ASSERT_TRUE(parse->Failed);
	}

	TEST(PiParser, TestCompoundFail2)
	{
		auto input = "[[]";
		shared_ptr<PiLexer> lex = make_shared<PiLexer>(input);
		lex->Process();
		KAI_TRACE_1(lex->Print());
		if (lex->Failed)
			KAI_TRACE_1(lex->Error);

		shared_ptr<PiParser> parse = make_shared<PiParser>(lex);
		parse->Run(Structure::Sequence);

		KAI_TRACE_1(parse->Print());
		ASSERT_FALSE(lex->Failed);
		ASSERT_TRUE(parse->Failed);
	}

	TEST(PiParser, TestCompoundFail3)
	{
		auto input = "]";
		shared_ptr<PiLexer> lex = make_shared<PiLexer>(input);
		lex->Process();
		KAI_TRACE_1(lex->Print());
		if (lex->Failed)
			KAI_TRACE_1(lex->Error);

		shared_ptr<PiParser> parse = make_shared<PiParser>(lex);
		parse->Run(Structure::Sequence);

		KAI_TRACE_1(parse->Print());
		ASSERT_FALSE(lex->Failed);
		ASSERT_TRUE(parse->Failed);
	}
}
