#include <KAI/Core/StringStream.h>
#include <KAI/Language/Common/Structure.h>

KAI_BEGIN

StringStream &operator<<(StringStream &S, const Structure &T)
{
	return << (int)T;
}

StringStream &operator>>(StringStream &S, Structure &T)
{
	int num;
	S >> num;
	T = (Structure)num;
	return S;
}

KAI_END
