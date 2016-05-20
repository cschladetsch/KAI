#pragma once

#include <KAI/Core/Config/Base.h>

KAI_BEGIN

template <class T>
int PointerToInt(T const *ptr)
{
	return (int)reinterpret_cast<__int64_t>(ptr);
}

KAI_END

//EOF

