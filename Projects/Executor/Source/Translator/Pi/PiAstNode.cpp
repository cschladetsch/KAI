#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Pi/PiToken.h"
#include "KAI/Translator/Pi/PiAstNode.h"

KAI_BEGIN

const char *ToString(PiAstNodeEnumType::Enum ty)
{
	switch (ty)
	{
		#define CASE(X) case PiAstNodeEnumType::Enum::X: return #X;
		CASE(None)
		CASE(Object)
		CASE(Operation)
		CASE(List)
		CASE(Map)
		CASE(Set)
		CASE(Array)
		CASE(Continuation)
	}

	static char b[100];
	_itoa_s(ty, b, 100, 10);
	return b;
}

std::ostream& operator<<(std::ostream& out, const PiAstNode &node)
{
	return out << node.ToString();
}

KAI_END
