#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/LexerBase.h"

KAI_BEGIN

LexerBase::LexerBase(const char *in)
	: input(in)
{
	if (input.empty())
		return;
}

void LexerBase::Construct()
{
	AddKeyWords();

	CreateLines();
	Run();
}

void LexerBase::CreateLines()
{
	if (input.back() != '\n')
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

bool LexerBase::Run()
{
	offset = 0;
	lineNumber = 0;

	while (!Failed && NextToken())
		;

	return !Failed;
}

char LexerBase::Current() const
{
	if (lineNumber == (int)lines.size())
		return 0;

	return Line()[offset];
}

Slice LexerBase::Gather(int (*filt)(int)) 
{
	int start = offset;
	while (filt(Next()))
		;

	return Slice(start, offset);
}

char LexerBase::Next()
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

char LexerBase::PeekBase() const
{
	if (EndOfLine())
		return 0;

	return Line()[offset + 1];
}

bool LexerBase::LexString()
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
				LexErrorBase("Bad escape sequence %c");
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

	// the +1 and -1 to remove the start and end double quote " characters
	AddStringToken(lineNumber, Slice(start + 1, offset - 1));

	return true;
}

KAI_END
