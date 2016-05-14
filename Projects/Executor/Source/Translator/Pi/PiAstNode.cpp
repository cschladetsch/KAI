#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Pi/PiToken.h"
#include "KAI/Translator/Pi/PiAstNode.h"

#include <stdlib.h>
#include <stdio.h>

KAI_BEGIN

const char *ToString(PiAstNodeEnumType::Enum ty)
{
	switch (ty)
	{
		#define CASE(X) case PiAstNodeEnumType::Enum::X: return #X;
		CASE(Program)
		CASE(None)
		CASE(Operation)
		CASE(List)
		CASE(Map)
		CASE(Set)
		CASE(Array)
		CASE(Continuation)
		CASE(TokenType)
	}

	static char b[100];
	sprintf(b, "%d", (int)ty);
	return b;
}

std::ostream& operator<<(std::ostream& out, const PiAstNode &node)
{
	return out << node.ToString();
}

KAI_END
