#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Rho/RhoToken.h"
#include "KAI/Translator/Rho/RhoAstNode.h"

KAI_BEGIN

const char *ToString(RhoAstNodes::Enum ty)
{
	switch (ty)
	{
		#define CASE(N) case RhoAstNodes::N : return #N;
		#define CASE_LOWER(N) case RhoAstNodes::N : return ToLower(#N);
		#define CASE_REPLACE(N, M) case RhoAstNodes::N : return M;
		CASE(None)
		CASE(Program)
		CASE(Ident)
		CASE(GetMember)
		CASE(Function)
		CASE(Block)
		CASE(Conditional)
		CASE(Assignment)
		CASE(Call)
		CASE(Increment)
		CASE(Decrement)
		CASE(TokenType)
		CASE(Positive)
		CASE(Negative)
		CASE(ArgList)
		CASE(IndexOp)
		CASE(ForEach)
		CASE(For)
		CASE(List)
		CASE(Map)
	}
	return "???";
}

KAI_END
