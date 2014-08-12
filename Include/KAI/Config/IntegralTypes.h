
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_CONFIG_INTEGRAL_TYPES_H
#	define KAI_CONFIG_INTEGRAL_TYPES_H

#define KAI_COMPILER_MSVC

KAI_BEGIN

#	ifdef KAI_COMPILER_MSVC

KAI_BEGIN
typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef __int8 uint8;
typedef __int16 uint16;
typedef __int32 uint32;
typedef __int64 uint64;
KAI_END

#	else
#		error "Add your compiler"
#	endif

KAI_END

#endif // KAI_CONFIG_INTEGRAL_TYPES_H

// EOF
