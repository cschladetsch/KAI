#pragma once

#include "KAI/Type/Traits.h"

KAI_BEGIN

struct Void
{ 
	virtual ~Void() { }
};

inline StringStream &operator<<(StringStream &S, Void  const &) { return S; }
inline StringStream &operator>>(StringStream &S, Void  &) { return S; }
inline BinaryStream &operator<<(BinaryStream &S, Void  const &) { return S; }
inline BinaryPacket &operator>>(BinaryPacket &S, Void  &) { return S; }

inline HashValue GetHash(Void) { return 0; }

namespace Type
{
	template <> struct Traits<void> 
		: TraitsBase<void, Number::Void, 0,/*TODO Properties::Streaming,*/ Void, Void &, const Void&> 
	{ 
		static const char *Name() 
		{ 
			typedef TraitsBase<void, Number::Void, Properties::Streaming, Void, Void &, const Void&> Tr;
			return Tr::Name();
		}
	};

	template <> struct Traits<meta::Null> : Traits<void> { };
	template <> struct Traits<Void> : Traits<void> { };

	//const char *Traits<void>::Name = "Void";
}

KAI_END

//EOF

