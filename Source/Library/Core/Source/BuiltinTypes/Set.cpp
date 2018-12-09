
#include "KAI/Core/BuiltinTypes.h"
#include <KAI/Core/Object/ClassBuilder.h>

KAI_BEGIN

void ObjectSet::Register(Registry &R)
{
    ClassBuilder<ObjectSet>(R, Label("Set"))
        ;
}

bool ObjectSet::Empty() const
{
    return objects.empty();
}

int ObjectSet::Size() const
{
    return static_cast<int>(objects.size());
}

bool ObjectSet::Destroy()
{
    Clear();
    return true;
}

void ObjectSet::Insert(Object const &Q)
{
    Append(Q);
}

void ObjectSet::Append(Object const &Q) 
{ 
    if (Attach(Q))
        objects.insert(Q); 
}

void ObjectSet::Clear() 
{ 
    while (!Empty())
        Erase(begin());
}

ObjectSet::iterator ObjectSet::Erase(iterator iter)
{
    if (iter->Exists())
        Detach(*iter);

    return objects.erase(iter);
}

ObjectSet::iterator ObjectSet::Erase(Object const &Q)
{
    iterator A = objects.find(Q);
    if (A != end())
        return Erase(A);

    KAI_THROW_1(UnknownObject, Q.GetHandle());
}

StringStream &operator<<(StringStream &S, const ObjectSet &T)
{
    return S << "Set: size=" << T.Size();
}

BinaryStream &operator<<(BinaryStream &S, const ObjectSet &T)
{
    KAI_UNUSED_2(S,T);
    KAI_NOT_IMPLEMENTED();
}

BinaryStream &operator>>(BinaryStream &S, ObjectSet &T)
{
    KAI_UNUSED_2(S,T);
    KAI_NOT_IMPLEMENTED();
}

KAI_END

