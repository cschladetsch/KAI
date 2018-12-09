#include <KAI/Core/Config/Base.h>
#include <KAI/Core/Type/Number.h>
#include <KAI/Core/BuiltinTypes.h>

KAI_BEGIN

// THESE ARE TERRIBLE HASH FUNCTIONS FOR CONTAINERS

HashValue GetHash(const Stack &S)
{
    return 42;
    // if (S.Empty())
    //     return Type::Number::Stack;
    // size_t h = 42;
    // for (const auto &v: S)
    // {
    //     h ^= v.GetClass().HashValue(v);
    // }
    // return h;//GetHash(S.Top());
}

HashValue GetHash(const Array &A)
{
    if (A.Empty())
        return Type::Number::Array;
    return GetHash(A.At(0));
}

HashValue GetHash(const Map &M)
{
    if (M.Empty())
        return Type::Number::Map;
    Map::const_iterator A = M.Begin();
    return GetHash(A->first) ^ GetHash(A->second);
}

HashValue GetHash(const String &S)
{
    HashValue hash = 5381;
    String::const_iterator A = S.begin(), B = S.end();
    for (; A != B; ++A)
        hash = ((hash << 5) + hash) + (int)*A;
    return hash;
}

KAI_END


