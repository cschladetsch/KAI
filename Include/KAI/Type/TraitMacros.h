#pragma once

KAI_BEGIN

#define KAI_TYPE_TRAITS_NOASSIGN(T, N, Ops) \
	namespace Type \
	{ \
		template <> \
		struct Traits<T> : TraitsBase<T, N, Ops> { static const char *Name; }; \
		const char *Traits<T>::Name = #T; \
	}

#define KAI_TYPE_TRAITS_NAMED(T, N, M, Ops) \
	namespace Type \
	{ \
		template <> \
		struct Traits<T> : TraitsBase<T, N, Ops> { static const char *Name; }; \
		const char *Traits<T>::Name = M; \
	}

// T = tje type
// N = tje number from ::kai::Type::Number
// P = operations allowed on the type
#define KAI_TYPE_TRAITS(T, N, Ops) \
	KAI_TYPE_TRAITS_NAMED(T, N, #T, Ops)

#define KAI_TYPE_TRAITS_BASIC(T, N) \
	KAI_TYPE_TRAITS(T,N,0)

#define KAI_TYPE_TRAITS_REFLECTED(T, N) \
	KAI_TYPE_TRAITS(T,N,Type::Properties::Reflected)

// TOD
//#define kai_type_traits_arithmetic(t, n) \
	//KAI_TYPE_TRAITS(T,N,Type::Properties::Arithmetic | Type::Properties::StringStreamInsert)

#define KAI_TYPE_TRAITS_BASIC_STRING_STREAM_INSERT(T,N) \
	KAI_TYPE_TRAITS(T,N,Type::Properties::StringStreamInsert)

KAI_TYPE_TRAITS(String, Number::String
	, Properties::Streaming | Properties::Plus | Properties::Equiv | Properties::Relational)

KAI_TYPE_TRAITS(Handle, Number::Handle, Properties::StringStreamInsert)

KAI_END

