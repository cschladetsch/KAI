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
	bool isQuoted = Current() == '\'';
	if (isQuoted)
		Next();
	Token tok(Token::Ident, *this, lineNumber, Gather(isalnum));

	if (!isQuoted)
	{
		auto kw = keyWords.find(tok.Text());
		if (kw != keyWords.end())
			tok.type = kw->second;
	}
	else
	{
		// include the quote char for quoted identifiers
		tok.slice.Start--;
	}

	tokens.push_back(tok);
	return true;
}

bool Lexer::NextToken()
{
	auto current = Current();
	if (current == 0)
		return false;

	if (isspace(current))
		return Add(Token::Whitespace, Gather(isspace));

	if (isalpha(current))
		return LexAlpha();	
	
	if (isdigit(current))
		return Add(Token::Int, Gather(isdigit));

	switch (current)
	{
	case '\'': return LexAlpha();
	case '-': return Add(Token::Minus);
	case '.': return Add(Token::Dot);
	case ',': return Add(Token::Comma);
	case '+': return AddIfNext('+', Token::Increment, Token::Plus);
	case '*': return Add(Token::Mul);
	case '/': return Add(Token::Divide);
	case ';': return Add(Token::Semi);
	case '{': return Add(Token::OpenBrace);
	case '}': return Add(Token::CloseBrace);
	case '(': return Add(Token::OpenParan);
	case ')': return Add(Token::CloseParan);
	case '=': return AddIfNext('=', Token::Equiv, Token::Assign);
	case '[': return Add(Token::OpenSquareBracket);
	case ']': return Add(Token::CloseSquareBracket);
	case '!': return AddIfNext('=', Token::NotEquiv, Token::Not);
	case '&': return AddIfNext('&', Token::And, Token::BitAnd);
	case '|': return AddIfNext('|', Token::Or, Token::BitOr);
	case '"': return LexString();
	}

	if (current == '<')
	{
		if (Peek() == '=')
		{
			return AddTwoCharOp(Token::LessEquiv);
		}
		return Add(Token::Less);
	}

	if (current == '>')
	{
		if (Peek() == '=')
		{
			return AddTwoCharOp(Token::GreaterEqiv);
		}
		return Add(Token::Greater);
	}

	LexError("Unrecognised %c");

	return false;
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
		err << lex.lines[n];
		if (n == tok.lineNumber)
		{
			for (int c = 0; c < tok.slice.Start; ++c)
				err << ' ';
			err << '^' << endl;
		}
	}
	err << ends;

	return err.str();
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
	Failed = true;
	Error = buffer;
	return false;
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
		Next();
	}
	Next();
	tokens.push_back(Token(Token::String, *this, lineNumber, Slice(start, offset)));
	return true;
}

void Lexer::LexError(const char *text)
{
	Fail(CreateError(Token(Token::None, *this, lineNumber, Slice(offset, offset)), text, Current()));
}

KAI_END
