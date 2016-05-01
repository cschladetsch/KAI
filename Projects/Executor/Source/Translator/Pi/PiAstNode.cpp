#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Pi/PiAstNode.h"

KAI_BEGIN

const char *PiAstNodeEnumType::ToString(Enum ty)
{
	switch (ty)
	{
#define CASE(X) case Enum::X: return #X;
		CASE(None);
		CASE(Anything);
	}
	return "???";
}

KAI_END
