#pragma once

#include "KAI/ExecutorPCH.h"

KAI_BEGIN

char TokenBase::operator[](int n) const
{
	return lexer->GetInput()[slice.Start + n];
}

std::string TokenBase::Text() const
{
	if (lexer == 0)
		return "";

	return std::move(lexer->GetLine(lineNumber).substr(slice.Start, slice.Length()));
}

KAI_END
