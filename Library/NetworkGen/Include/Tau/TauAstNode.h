#pragma once

#include <KAI/Language/Common/AstNodeBase.h>
#include "Tau/TauToken.h"

TAU_BEGIN

struct TauAstEnumType
{
	enum Enum
	{
		None = 0,
		Namespace = 1,
		Class = 2,
		Property = 3,
		Method = 4,

		// req.
		TokenType = 5,
	};

	struct Node : AstNodeBase<TauToken, TauAstEnumType>
	{
		typedef AstNodeBase<TauToken, TauAstEnumType> Parent;
		using typename Parent::Token;
		KAI_NAMESPACE(Object) object;

		Node() : Parent(Enum::None) {  }
		Node(Enum e) : Parent(e) { }
		Node(Enum e, Token &tok) : Parent(e, tok) { }
		Node(Parent::Token tok) : Parent(tok) { }
	};

	static const char *ToString(Enum);
};

TAU_END
