#pragma once

#include "KAI/ExecutorPCH.h"

KAI_BEGIN

int IsSpaceChar(int ch);

// Common to all lexers with different token types
struct LexerBase : Process
{
	LexerBase(const char *);
	typedef std::vector<std::string> Lines;

	const Lines &GetLines() const { return lines; }
	const std::string GetInput() const { return input; }
	int GetOffset() const { return offset; }
	int GetLineNumber() const { return offset; }
	const std::string &Line() const;

protected:
	void Construct();

	Lines lines;
	std::string input;
	int offset, lineNumber;

	virtual void AddKeyWords() = 0;
	virtual void LexErrorBase(const char *) = 0;
	virtual void Terminate() = 0;
	virtual bool NextToken() = 0;
	virtual void AddStringToken(int ineNumber , Slice slice) = 0;

	void CreateLines();
	bool Run();
	bool LexString();

	char Current() const;
	char Next();
	bool EndOfLine() const;
	char Peek() const;
	char PeekBase() const;

	Slice Gather(int(*filt)(int ch));
};

KAI_END
