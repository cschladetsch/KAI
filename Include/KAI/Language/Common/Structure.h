#pragma once

#include <KAI/Core/Base.h>

KAI_BEGIN

enum class Structure
{
	None,
	Single,
	Sequence,
	Module,
	Statement,
	Function,
	Expression,
	Program,

	Namespace,
	Class,
};

StringStream &operator<<(StringStream &S, const Structure& T);
StringStream &operator>>(StringStream &S, const Structure T);

KAI_END

