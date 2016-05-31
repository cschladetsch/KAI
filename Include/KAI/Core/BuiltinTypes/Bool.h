#pragma once

#include <KAI/Core/Type.h>

KAI_BEGIN

KAI_TYPE_TRAITS(bool, Number::Bool
	, Properties::Streaming 
	| Properties::Assign)

StringStream &operator<<(StringStream &, bool);
StringStream &operator>>(StringStream &, bool &);
BinaryStream &operator<<(BinaryStream &, bool);
BinaryStream &operator>>(BinaryStream &, bool &);

KAI_END
