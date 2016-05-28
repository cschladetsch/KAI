#include "Tau/TauAstNode.h"

KAI_BEGIN

const char *TauAstEnumType::ToString(Enum ty)
{
	switch (ty)
	{
		#define CASE(N) case TauAstNodes::N : return #N;
		#define CASE_LOWER(N) case TauAstNodes::N : return ToLower(#N);
		#define CASE_REPLACE(N, M) case TauAstNodes::N : return M;
	}
	return "???";
}

KAI_END
