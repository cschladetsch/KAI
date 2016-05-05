#pragma once

#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/CommonBase.h"

KAI_BEGIN

int IsSpaceChar(int ch);

// Common to all lexers with different token types
class LexerBase: public CommonBase
{
public:
	typedef std::vector<std::string> Lines;

	LexerBase(const char *);

	const std::string &GetLine(size_t n) const
	{
		if (lines.empty() || n < 0 || n >= lines.size())
			KAI_THROW_2(OutOfBounds, n, 0);
		return lines[n];
	}

	const Lines &GetLines() const { return lines; }
	const std::string &GetInput() const { return input; }
	int GetOffset() const { return offset; }
	int GetLineNumber() const { return offset; }
	const std::string &Line() const;

protected:
	Lines lines;
	std::string input;
	int offset, lineNumber;

	void CreateLines();
	bool LexString();
	char Current() const;
	char Next();
	bool EndOfLine() const;
	char Peek() const;
	char PeekBase() const;

	virtual void LexErrorBase(const char *msg) = 0;
	virtual void AddStringToken(int lineNumber, Slice slice) = 0;

	Slice Gather(int(*filt)(int ch));
};

KAI_END

