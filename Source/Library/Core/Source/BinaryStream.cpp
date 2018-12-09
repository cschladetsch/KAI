
#include "KAI/Core/BinaryStream.h"
#include "KAI/Core/Object.h"

KAI_BEGIN

BinaryStream &operator<<(BinaryStream &S, const BinaryPacket &T)
{
    KAI_UNUSED_2(S,T);
    KAI_NOT_IMPLEMENTED();
}

BinaryStream &operator<<(BinaryStream &S, const BinaryStream &T)
{
    KAI_UNUSED_2(S,T);
    KAI_NOT_IMPLEMENTED();
}

void BinaryStream::Register(Registry &registry)
{
    ClassBuilder<BinaryStream>(registry, Label(Type::Traits<BinaryStream>::Name()))
        .Methods
                ("Size", &BinaryStream::Size)
                ("Clear", &BinaryStream::Clear)
        ;
}

KAI_END
