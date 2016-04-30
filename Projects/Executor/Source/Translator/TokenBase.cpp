#pragma once

#include "KAI/ExecutorPCH.h"

KAI_BEGIN

char TokenBase::operator[](int n) const
{
	return lexer->input[slice.Start + n];
}

std::string TokenBase::Text() const
{
	if (lexer == 0)
		return "";

	return std::move(lexer->lines[lineNumber].substr(slice.Start, slice.Length()));
}

KAI_END
