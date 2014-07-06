
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_BUILTIN_TYPES_VOID_H
#	define KAI_BUILTIN_TYPES_VOID_H

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
	template <> struct Traits<void> : TraitsBase<void, Number::Void, Properties::Streaming, None, Void , Void &, const Void&> { };
	template <> struct Traits<meta::Null> : Traits<void> { };
	template <> struct Traits<Void> : Traits<void> { };

	const char *Traits<void>::Name = "Void";
}

KAI_END

#endif // KAI_BUILTIN_TYPES_VOID_H

//EOF

