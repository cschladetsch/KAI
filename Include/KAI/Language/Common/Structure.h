#pragma once

#include <KAI/Core/Base.h>

KAI_BEGIN

enum class Structure
{
	None,
	Single,
	Sequence,
	Modulue,
	Statement,
	Function,
	Expression,
	Program,

	Namespace,
	Class,
};

KAI_END
