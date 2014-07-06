
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_BUILTIN_TYPES_PAIR_H
#	define KAI_BUILTIN_TYPES_PAIR_H

KAI_BEGIN

struct Pair
{
	Object first, second;
	Pair() {}
	Pair(const Object &A, const Object &B) : first(A), second(B) { }
	Pair(nstd::pair<Object, Object> const &P) : first(P.first), second(P.second) { }

	static void Register(Registry &);

	friend bool operator<(Pair const &A, Pair const &B)
	{
		return A.first < B.first || (!(B.first < B.first) && A.second < B.second);
	}
	friend bool operator==(Pair const &A, Pair const &B)
	{
		return A.first == B.first && A.second == B.second;
	}
};

StringStream &operator<<(StringStream &S, Pair const &);
StringStream &operator>>(StringStream &S, Pair &);
BinaryStream &operator<<(BinaryStream &S, Pair const &);
BinaryPacket &operator>>(BinaryPacket &S, Pair &);

HashValue GetHash(const Pair &);

KAI_TYPE_TRAITS(Pair, Type::Number::Pair
	, Type::Properties::Streaming 
	| Type::Properties::Equiv 
	| Type::Properties::Assign
	| Type::Properties::Less);

KAI_END

#endif // KAI_BUILTIN_TYPES_PAIR_H

//EOF

