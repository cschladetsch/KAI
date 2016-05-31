#include <KAI/Language/Pi/PiLexer.h>

using namespace std;

KAI_BEGIN

void PiLexer::AddKeyWords()
{
	keyWords["if"] = Enum::If;
	keyWords["for"] = Enum::For;
	keyWords["true"] = Enum::True;
	keyWords["false"] = Enum::False;
	keyWords["self"] = Enum::Self;
	keyWords["while"] = Enum::While;
	keyWords["assert"] = Enum::Assert;
	keyWords["div"] = Enum::Divide;
	keyWords["rho"] = Enum::ToRho;
	keyWords["rho{"] = Enum::ToRhoSequence;

	keyWords["drop"] = Enum::Drop;
	keyWords["dup"] = Enum::Dup;
	keyWords["pick"] = Enum::PickN;
	keyWords["over"] = Enum::Over;
	keyWords["swap"] = Enum::Swap;
	keyWords["rot"] = Enum::Rot;
	keyWords["rotn"] = Enum::RotN;
	keyWords["toarray"] = Enum::ToArray;
	keyWords["gc"] = Enum::GarbageCollect;
	keyWords["clear"] = Enum::Clear;
	keyWords["expand"] = Enum::Expand;
	keyWords["cd"] = Enum::ChangeFolder;
	keyWords["pwd"] = Enum::PrintFolder;
	keyWords["type"] = Enum::GetType;
}

bool PiLexer::QuotedIdent()
{
	auto start = offset;
	Next();
	if (!LexAlpha())
		return false;

	PiToken id = tokens.back();
	tokens.pop_back();
	Add(PiTokens::QuotedIdent, Slice(start + 1, offset));

	return true;
}

bool PiLexer::NextToken()
{
	char current = Current();
	if (current == 0)
		return false;

	if (isalpha(current))
		return LexAlpha();

	if (isdigit(current))
		return Add(Enum::Int, Gather(isdigit));

	switch (current)
	{
	case '\'': return QuotedIdent();
	case '{': return Add(Enum::OpenBrace);
	case '}': return Add(Enum::CloseBrace);
	case '(': return Add(Enum::OpenParan);
	case ')': return Add(Enum::CloseParan);
	case ':': return Add(Enum::Colon);
	case ' ': return Add(Enum::Whitespace, Gather(IsSpaceChar));
	case '@': return Add(Enum::Lookup);
	case ',': return Add(Enum::Comma);
	case '#': return Add(Enum::Store);
	case '*': return Add(Enum::Mul);
	case '[': return Add(Enum::OpenSquareBracket);
	case ']': return Add(Enum::CloseSquareBracket);
	case '=': return AddIfNext('=', Enum::Equiv, Enum::Assign);
		case '!': return Add(Enum::Replace);
	//case '!': return AddIfNext('=', Enum::NotEquiv, Enum::Not);
	//case '&': return AddIfNext('&', Enum::And, Enum::BitAnd);
		case '&': return Add(Enum::Suspend);
	case '|': return AddIfNext('|', Enum::Or, Enum::BitOr);
	case '<': return AddIfNext('=', Enum::LessEquiv, Enum::Less);
	case '>': return AddIfNext('=', Enum::GreaterEquiv, Enum::Greater);
	case '"': return LexString();
	case '\t': return Add(Enum::Tab);
	case '\n': return Add(Enum::NewLine);
	case '-':
		if (Peek() == '-')
			return AddTwoCharOp(Enum::Decrement);
		if (Peek() == '=')
			return AddTwoCharOp(Enum::MinusAssign);
		return Add(Enum::Minus);

	case '.':
		if (Peek() == '.')
		{
			Next();
			if (Peek() == '.')
			{
				Next();
				return Add(Enum::Resume, 3);
			}
			return Fail("Two dots doesn't work");
		}
		return Add(Enum::Dot);

	case '+':
		if (Peek() == '+')
			return AddTwoCharOp(Enum::Increment);
		if (Peek() == '=')
			return AddTwoCharOp(Enum::PlusAssign);
		return Add(Enum::Plus);

	case '/':
		if (Peek() == '/')
		{
			Next();
			int start = offset;
			while (Next() != '\n')
				;
			return Add(Enum::Comment, offset - start);
		}
		return ParsePathname();
	}

	LexError("Unrecognised %c");

	return false;
}

void PiLexer::Terminate()
{
	Add(Enum::None, 0);
}

	bool PiLexer::ParsePathname()
	{
		return false;
	}


KAI_END
