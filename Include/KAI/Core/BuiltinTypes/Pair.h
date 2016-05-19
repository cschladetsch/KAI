#pragma once

#include <KAI/Core/Config/Base.h>
#include <KAI/Core/Object.h>

KAI_BEGIN

struct Pair
{
	Object first, second;

	Pair() {}
	Pair(const Object &A, const Object &B) : first(A), second(B) { }
	Pair(std::pair<Object, Object> const &P) : first(P.first), second(P.second) { }

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

KAI_TYPE_TRAITS(Pair, Number::Pair
	, Properties::Streaming 
	| Properties::Equiv 
	| Properties::Assign
	| Properties::Less);

KAI_END

