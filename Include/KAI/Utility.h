

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_UTILITY_H
#	define KAI_UTILITY_H

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
