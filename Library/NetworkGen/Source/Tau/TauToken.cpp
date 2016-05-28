#include "Tau/TauToken.h"

TAU_BEGIN

const char *TauTokenEnumType::ToString(Enum val)
{
	switch (val)
	{
		#define CASE(N) case TauToken::N : return #N;
		#define CASE_LOWER(N) case TauToken::N : return ToLower(#N);
		#define CASE_REPLACE(N, M) case TauToken::N : return M;

	}

	return "Unnamed";
}

std::ostream &operator<<(std::ostream &out, TauToken const &node)
{
	if (node.type == TauTokenEnumType::None)
		return out;

	out << TauTokenEnumType::ToString(node.type);
	switch (node.type)
	{
	case TauTokenEnumType::Int:
	case TauTokenEnumType::String:
	case TauTokenEnumType::Ident:
		out << "=" << node.Text();
	}

	return out;
}

TAU_END
