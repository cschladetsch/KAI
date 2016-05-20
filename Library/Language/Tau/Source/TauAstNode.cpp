#include "KAI/Executor/Executor.h"
#include "KAI/Language/Tau/TauToken.h"
#include "KAI/Language/Tau/TauAstNode.h"

#include <stdlib.h>
#include <stdio.h>

KAI_BEGIN

	const char *TauAstEnumType::ToString(Enum ty)
	{
		switch (ty)
		{
			#define CASE(X) case TauAstEnumType::Enum::X: return #X;
			CASE(None)
			CASE(Property)
			CASE(Method)
			CASE(Class)
			CASE(Ctor)
			CASE(Dtor)
			default:
				return "TauAstNode";
		}

	}

	std::ostream& operator<<(std::ostream& out, const TauAstEnumType::Node &node)
	{
		return out << node.ToString();
	}

KAI_END


