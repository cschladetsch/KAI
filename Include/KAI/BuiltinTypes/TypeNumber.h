
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_BUILTIN_TYPES_TYPE_NUMBER_H
#	define KAI_BUILTIN_TYPES_TYPE_NUMBER_H

KAI_BEGIN

KAI_TYPE_TRAITS(Type::Number, Type::Number::TypeNumber, Type::Properties::StringStreamInsert);

StringStream &operator<<(StringStream &, Type::Number);
BinaryStream &operator<<(BinaryStream &, Type::Number);
BinaryPacket &operator>>(BinaryPacket &, Type::Number &);

KAI_END

#include KAI_TEST(X)

#endif // KAI_BUILTIN_TYPES_TYPE_NUMBER_H

//EOF

