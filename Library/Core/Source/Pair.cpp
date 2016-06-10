#include <KAI/Core/BuiltinTypes.h>

KAI_BEGIN

StringStream &operator<<(StringStream &S, const Pair& P)
{
	return S << P.first << " " << P.second;
}

StringStream &operator>>(StringStream &S, Pair& P)
{
	return S >> P.first >> P.second;
}

BinaryStream &operator<<(BinaryStream &S, const Pair& P)
{
	return S << P.first << P.second;
}

BinaryStream &operator>>(BinaryStream &S, Pair& P)
{
	return S >> P.first >> P.second;
}

KAI_END

