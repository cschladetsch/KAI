#include "KAI/KAI.h"

#include <iostream>
#include <strstream>
#include <stdarg.h>

#include "KAI/Translator/Lexer.h"

using namespace std;

KAI_BEGIN

Lexer::Lexer(const char *in)
	: input(in)
{
	CreateLines();
	AddKeywords();
	Run();
}

void Lexer::AddKeywords()
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

bool Lexer::Run()
{
	offset = 0;
	lineNumber = 0;

	while (!Failed && NextToken())
		;

	return Add(Token::None, 0);
}

void Lexer::Print() const
{
	//std::copy(tokens.begin(), tokens.end(), ostream_iterator<Token>(std::cout, " "));
	for (auto tok : tokens)
		std::cout << tok << " ";

	std::cout << std::endl;
}

Slice Lexer::Gather(int (*filt)(int)) 
{
	int start = offset;
	while (filt(Next()))
		;

	return Slice(start, offset);
}

bool Lexer::Add(Token::Type type, Slice slice)
{
	tokens.push_back(Token(type, *this, lineNumber, slice));
	return true;
}

bool Lexer::Add(Token::Type type, int len)
{
	Add(type, Slice(offset, offset + len));
	while (len--) 
		Next();

	return true;
}

bool Lexer::LexAlpha()
{
	Token tok(Token::Ident, *this, lineNumber, Gather(isalnum));

	auto kw = keyWords.find(tok.Text());
	if (kw != keyWords.end())
		tok.type = kw->second;

	tokens.push_back(tok);

	return true;
}

int IsSpaceChar(int ch)
{
	return ch == ' ';
}

bool Lexer::NextToken()
{
	auto current = Current();
	if (current == 0)
		return false;

	if (isalpha(current))
		return LexAlpha();	
	
	if (isdigit(current))
		return Add(Token::Int, Gather(isdigit));

	switch (current)
	{
	case '\t': return Add(Token::Tab);
	case '\n': return Add(Token::NewLine);
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

bool Process::Fail(const std::string &err)
{
	Failed = true;
	Error = err;

	return false;
}

bool Process::Fail(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	char buffer[1000];
	vsprintf_s(buffer, fmt, ap);

	return Fail(std::string(buffer));
}

char Lexer::Current() const
{
	if (lineNumber == (int)lines.size())
		return 0;

	return Line()[offset];
}

const std::string &Lexer::Line() const
{
	return lines[lineNumber];
}

char Lexer::Next()
{
	if (EndOfLine())
	{
		offset = 0;
		++lineNumber;
	}
	else
		++offset;

	if (lineNumber == (int)lines.size())
		return 0;

	return Line()[offset];
}

char Lexer::Peek() const
{
	if (EndOfLine())
		return 0;

	return Line()[offset + 1];
}

void Lexer::CreateLines()
{
	// ensure we end with a newline.
	input.push_back('\n');

	size_t lineStart = 0;
	for (size_t n = 0; n < input.size(); ++n)
	{
		if (input[n] == '\n')
		{
			lines.push_back(input.substr(lineStart, n - lineStart + 1));
			lineStart = n + 1;
		}
	}
}

bool Lexer::EndOfLine() const
{
	return offset == (int)Line().size() - 1;
}

bool Lexer::AddIfNext(char ch, Token::Type thenType, Token::Type elseType)
{
	if (Peek() == ch)
	{
		Next();
		return Add(thenType, 2);
	}

	return Add(elseType, 1);
}

bool Lexer::AddTwoCharOp(Token::Type ty)
{
	Add(ty, 2);
	Next();

	return true;
}

bool Lexer::LexString()
{
	int start = offset;
	Next();
	while (!Failed && Current() != '"')
	{
		if (Current() == '\\')
		{
			switch (Next())
			{
			case '"':
			case 'n':
			case 't':
				break;

			default:
				LexError("Bad escape sequence %c");
				return false;
			}
		}

		if (Peek() == 0)
		{
			Fail("Bad string literal");
			return false;
		}

		Next();
	}

	Next();

	// the +1 and -1 to remove the start and end double quote characters
	tokens.push_back(Token(Token::String, *this, lineNumber, Slice(start + 1, offset - 1)));
	return true;
}

void Lexer::LexError(const char *text)
{
	Fail(CreateError(Token(Token::None, *this, lineNumber, Slice(offset, offset)), text, Current()));
}

std::string Lexer::CreateError(Token tok, const char *fmt, ...)
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

	const Lexer &lex = *tok.lexer;
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
				if (lex.lines[tok.lineNumber][ch] == '\t')
					err << "    ";
				else if (ch == tok.slice.Start)
				{
					err << '^';
					break;
				}
			}

			err << endl;
		}
	}

	err << ends;

	return err.str();
}

KAI_END

