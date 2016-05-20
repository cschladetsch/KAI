#pragma once

#include <KAI/Language/Common/AstNodeBase.h>
#include "KAI/Language/Tau/TauToken.h"

KAI_BEGIN

struct TauAstEnumType
{
	enum Enum
	{
		None = 0,
		Property = 1,
		Method = 2,
		Class = 3,
		Ctor = 4,
		Dtor = 5,

	};

	struct Node : AstNodeBase<TauToken, TauAstEnumType>
	{
		typedef AstNodeBase<TauAstEnumType, TauAstEnumType> Parent;
		using typename Parent::Token;
		KAI_NAMESPACE(Object) object;

		Node() : Parent(Enum::None) {  }
		Node(Enum e) : Parent(e) { }
		Node(Enum e, Token &tok) : Parent(e, tok) { }
		Node(Parent::Token tok) : Parent(tok) { }
	};
};


KAI_END
