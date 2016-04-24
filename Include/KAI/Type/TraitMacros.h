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
		const char *Traits<T>::Name = #T; \
	}

#define KAI_TYPE_TRAITS(T, N, Ops) \
	KAI_TYPE_TRAITS_NAMED(T, N, #T, Ops | Properties::Assign)

#define KAI_TYPE_TRAITS_BASIC(T, N) \
	KAI_TYPE_TRAITS(T,N,0)

#define KAI_TYPE_TRAITS_REFLECTED(T, N) \
	KAI_TYPE_TRAITS(T,N,Type::Properties::Reflected)

#define KAI_TYPE_TRAITS_ARITHMETIC(T, N) \
	KAI_TYPE_TRAITS(T,N,Type::Properties::Arithmetic | Type::Properties::StringStreamInsert)

#define KAI_TYPE_TRAITS_BASIC_STRING_STREAM_INSERT(T,N) \
	KAI_TYPE_TRAITS(T,N,Type::Properties::StringStreamInsert)

KAI_END

