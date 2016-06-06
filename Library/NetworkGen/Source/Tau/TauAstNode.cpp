#include "Tau/TauAstNode.h"

TAU_BEGIN

const char *TauAstEnumType::ToString(Enum ty)
{
	switch (ty)
	{
		#define CASE(N) case TauAstEnumType::N : return #N;
		#define CASE_LOWER(N) case TauAstEnumType::N : return ToLower(#N);
		#define CASE_REPLACE(N, M) case TauAstEnumType::N : return M;
		CASE(Namespace)
		CASE(Class)
		CASE(Property)
		CASE(Method)
		CASE(TokenType)
		CASE(Module)
	}
}

TAU_END
