#include "KAI/ExecutorPCH.h"

#include <strstream>
#include <stdarg.h>
#include <algorithm>

using namespace std;

KAI_BEGIN

int IsSpaceChar(int ch)
{
	return ch == ' ';
}

template <class T>
LexerCommon<T>::LexerCommon(const char *in)
	: LexerBase(in)
{
}

template <class T>
void LexerCommon<T>::AddKeywords()
{
	keyWords["if"] = Token::If;
	keyWords["else"] = Token::Else;
	keyWords["for"] = Token::For;
	keyWords["true"] = Token::True;
	keyWords["false"] = Token::False;
	keyWords["return"] = Token::Return;
	keyWords["self"] = Token::Self;
	keyWords["fun"] = Token::Fun;
	keyWords["yield"] = Token::Yield;
	keyWords["in"] = Token::In;
	keyWords["while"] = Token::While;
	keyWords["assert"] = Token::Assert;
}

template <class T>
bool LexerCommon<T>::NextToken()
{
	char current = Current();
	if (current == 0)
		return false;

	if (isalpha(current))
		return LexAlpha();

	if (isdigit(current))
		return Add(Token::Int, Gather(isdigit));

	switch (current)
	{
	case ';': return Add(Token::Semi);
	case '{': return Add(Token::OpenBrace);
	case '}': return Add(Token::CloseBrace);
	case '(': return Add(Token::OpenParan);
	case ')': return Add(Token::CloseParan);
	case ':': return Add(Token::Colon);
	case ' ': return Add(Token::Whitespace, Gather(IsSpaceChar));
	case '@': return Add(Token::Lookup);
	case ',': return Add(Token::Comma);
	case '*': return Add(Token::Mul);
	case '[': return Add(Token::OpenSquareBracket);
	case ']': return Add(Token::CloseSquareBracket);
	case '=': return AddIfNext('=', Token::Equiv, Token::Assign);
	case '!': return AddIfNext('=', Token::NotEquiv, Token::Not);
	case '&': return AddIfNext('&', Token::And, Token::BitAnd);
	case '|': return AddIfNext('|', Token::Or, Token::BitOr);
	case '<': return AddIfNext('=', Token::LessEquiv, Token::Less);
	case '>': return AddIfNext('=', Token::GreaterEquiv, Token::Greater);
	case '"': return LexString();
	case '\'': return LexAlpha();
	case '\t': return Add(Token::Tab);
	case '\n': return Add(Token::NewLine);
	case '-':
		if (Peek() == '-')
			return AddTwoCharOp(Token::Decrement);
		if (Peek() == '=')
			return AddTwoCharOp(Token::MinusAssign);
		return Add(Token::Minus);

	case '.':
		if (Peek() == '.')
		{
			Next();
			if (Peek() == '.')
			{
				Next();
				return Add(Token::Replace, 3);
			}
			return Fail("Two dots doesn't work");
		}
		return Add(Token::Dot);

	case '+':
		if (Peek() == '+')
			return AddTwoCharOp(Token::Increment);
		if (Peek() == '=')
			return AddTwoCharOp(Token::PlusAssign);
		return Add(Token::Plus);

	case '/':
		if (Peek() == '/')
		{
			Next();
			int start = offset;
			while (Next() != '\n')
				;
			return Add(Token::Comment, offset - start);
		}
		return Add(Token::Divide);
	}

	LexError("Unrecognised %c");

	return false;
}

template <class T>
bool LexerCommon<T>::LexAlpha()
{
	Token tok(Token::Ident, *this, lineNumber, Gather(isalnum));

	auto kw = keyWords.find(tok.Text());
	if (kw != keyWords.end())
		tok.type = kw->second;

	tokens.push_back(tok);

	return true;
}

const std::string &LexerBase::Line() const
{
	return lines[lineNumber];
}

bool LexerBase::EndOfLine() const
{
	return offset == (int)Line().size() - 1;
}

template <class T>
bool LexerCommon<T>::AddIfNext(char ch, TokenEnum thenType, TokenEnum elseType)
{
	if (Peek() == ch)
	{
		Next();
		return Add(thenType, 2);
	}

	return Add(elseType, 1);
}

template <class T>
bool LexerCommon<T>::AddTwoCharOp(TokenEnum ty)
{
	Add(ty, 2);
	Next();

	return true;
}

template <class T>
void LexerCommon<T>::LexError(const char *text)
{
	Fail(CreateErrorMessage(Token(Token::None, *this, lineNumber, Slice(offset, offset)), text, Current()));
}

template <class T>
bool LexerCommon<T>::Add(TokenEnum type, int len)
{
	Add(type, Slice(offset, offset + len));
	while (len--) 
		Next();

	return true;
}

template <class T>
bool LexerCommon<T>::Add(TokenEnum type, Slice slice)
{
	tokens.push_back(Token(type, *this, lineNumber, slice));
	return true;
}

template <class T>
static std::string LexerCommon<T>::CreateErrorMessage(typename LexerCommon<T>::Token tok, const char *fmt, ...)
{
	char buff0[2000];
	va_list ap;
	va_start(ap, fmt);
	vsprintf_s(buff0, fmt, ap);

	const char *fmt1 = "%s(%d):[%d]: %s\n";
	char buff[2000];
	sprintf_s(buff, fmt1, "", tok.lineNumber, tok.slice.Start, buff0);
	int beforeContext = 1;
	int afterContext = 0;

	const LexerCommon &lex = *tok.lexer;
	int start = std::max(0, tok.lineNumber - beforeContext);
	int end = std::min((int)lex.lines.size() - 1, tok.lineNumber + afterContext);

	strstream err;
	err << buff << endl;
	for (int n = start; n <= end; ++n)
	{
		for (auto ch : lex.lines[n])
		{
			if (ch == '\t')
				err << "    ";
			else
				err << ch;
		}

		if (n == tok.lineNumber)
		{
			for (int ch = 0; ch < (int)lex.lines[n].size(); ++ch)
			{
				if (ch == tok.slice.Start)
				{
					err << '^';
					break;
				}

				auto c = lex.lines[tok.lineNumber][ch];
				if (c == '\t')
					err << "    ";
				else
					err << ' ';
			}

			err << endl;
		}
	}

	err << ends;

	return err.str();
}

template <class T>
void LexerCommon<T>::Print() const
{
	for (auto tok : tokens)
		std::cout << tok << " ";

	std::cout << std::endl;
}

template <class  T>
void LexerCommon<T>::Terminate()
{
	return Add(TokenEnum::None, 0);
}

KAI_END
