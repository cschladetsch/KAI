#pragma once

#include <KAI/Core/Config/Base.h>
KAI_BEGIN

template <class T>
int PointerToInt(T const *ptr)
{
	// TODO: wtf
	return (int)reinterpret_cast<int64>(ptr);
}

KAI_END

#endif // KAI_UTILITY_H

//EOF
