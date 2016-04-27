#pragma once

KAI_BEGIN

#define KAI_TYPE_TRAITS_NAMED(T, N, M, Ops) \
	namespace Type \
	{ \
		template <> \
		struct Traits<T> : TraitsBase<T, N, Ops> { \
			typedef TraitsBase<T,N,Ops> Tr; \
			using typename Tr::Store; \
			using typename Tr::Reference; \
			using typename Tr::ConstReference; \
			static const char *Name() \
			{ \
				typedef TraitsBase<T,N,Ops> Tr; \
				return Tr::Name(); \
			} \
		}; \
	}

// T = the type
// N = the number from ::kai::Type::Number
// P = operations allowed on the type
#define KAI_TYPE_TRAITS(T, N, Ops) \
	KAI_TYPE_TRAITS_NAMED(T, N, #T, Ops)

#define KAI_TYPE_TRAITS_BASIC(T, N) \
	KAI_TYPE_TRAITS(T,N,0)

#define KAI_TYPE_TRAITS_REFLECTED(T, N) \
	KAI_TYPE_TRAITS(T,N,Type::Properties::Reflected)

#define KAI_TYPE_TRAITS_BASIC_STRING_STREAM_INSERT(T,N) \
	KAI_TYPE_TRAITS(T,N,Type::Properties::StringStreamInsert)

// need these defined before they are fully defined for forward references to their Traits
KAI_TYPE_TRAITS(String, Number::String, Properties::Streaming | Properties::Plus | Properties::Equiv | Properties::Relational)
KAI_TYPE_TRAITS(Handle, Number::Handle, Properties::StringStreamInsert)

KAI_TYPE_TRAITS(::kai::Type::Number, ::kai::Type::Number::TypeNumber, ::kai::Type::Properties::StringStreamInsert);

StringStream &operator<<(StringStream &, Type::Number);
BinaryStream &operator<<(BinaryStream &, Type::Number);
BinaryPacket &operator >> (BinaryPacket &, Type::Number &);

KAI_END