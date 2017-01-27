#include "KAI/Language/Pi/Pi.h"
#include "TestBase.h"
#include <algorithm>

using namespace kai;
using namespace std;

namespace
{
	bool IsWhitespace(PiToken t)
	{
		switch (t.type)
		{
		case PiTokens::Enum::Whitespace: return true;
		case PiTokens::Enum::Tab: return true;
		case PiTokens::Enum::NewLine: return true;
		}
		return false;
	}

	TEST(TestPiTokeniser, Tokenising)
	{
		auto input = "1 1 +		\n   \t \"hello, world\"		{ 1 2 * } [ 3 4 5 ]";
		std::array<PiTokens::Enum, 14> types =
		{
			PiTokens::Int,
			PiTokens::Plus,
			PiTokens::String,
			PiTokens::OpenBrace,
			PiTokens::Int,
			PiTokens::Int,
			PiTokens::Mul,
			PiTokens::CloseBrace,
			PiTokens::OpenSquareBracket,
			PiTokens::Int,
			PiTokens::Int,
			PiTokens::Int,
			PiTokens::CloseSquareBracket,

			//PiTokens::NewLine, usually added  automatically, but is stripped locally
			PiTokens::None,
		};

		Registry r;
		auto lex = make_shared<PiLexer>(input, r);
		lex->Process();
		auto tokens = lex->GetTokens();

		// remove all whitespace
		tokens.erase(remove_if(tokens.begin(), tokens.end(), [](auto t)->bool { return IsWhitespace(t); }), tokens.end());

		stringstream out;
		for (size_t m = 0; m < tokens.size(); ++m)
			out << m << ": " << tokens[m] << endl;
		KAI_TRACE_1(out.str());

		EXPECT_EQ(tokens.size(), types.size());

		for (size_t n = 0; n < tokens.size(); ++n)
			EXPECT_EQ(tokens[n].type, types[n]);
	}
}

