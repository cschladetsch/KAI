#pragma once

KAI_BEGIN

KAI_TYPE_TRAITS(Type::Number, Type::Number::TypeNumber, Type::Properties::StringStreamInsert);

StringStream &operator<<(StringStream &, Type::Number);
BinaryStream &operator<<(BinaryStream &, Type::Number);
BinaryPacket &operator>>(BinaryPacket &, Type::Number &);

KAI_END

//#include KAI_TEST(X)
