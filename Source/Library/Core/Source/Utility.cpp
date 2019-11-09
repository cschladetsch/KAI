#include <iostream>
#include <fstream>

#include "KAI/Core/BuiltinTypes.h"
#include "KAI/Core/Value.h"
#include "KAI/Core/Object/Reflected.h"
#include "KAI/Executor/Operation.h"
#include "KAI/Executor/Continuation.h"
#include <KAI/Core/BuiltinTypes/Dictionary.h>
#include <KAI/Core/Object/PropertyBase.h>
#include <KAI/Core/Object/ClassBase.h>
#include <KAI/Core/Exception.h>

KAI_BEGIN

const char *ToLower(const char *text)
{
    char *out = (char *)(malloc(strlen(text) + 1));
    size_t n;
    for (n = 0; n < strlen(text); ++n)
        out[n] = (char)tolower(text[n]);

    out[n] = 0;
    return out;
}

StringStream& operator<<(StringStream& S, ObjectColor::Color C)
{
    switch (C)
    {
    case ObjectColor::White:
        return S << "White";

    case ObjectColor::Grey:
        return S << "Grey";

    case ObjectColor::Black:
        return S << "Black";
    }

    return S << "UnknownColor";
}

void ToStringStream(const Object &Q, StringStream &S, int level)
{
    if (!Q.Valid())
    {
        S << "[Invalid]\n";
        return;
    }

    const StorageBase &base = Q.GetStorageBase();

    if (Q.GetClass()->HasTraitsProperty(Type::Properties::StringStreamInsert))
        Q.GetClass()->Insert(S, base);
}

void ToXmlStream(const Object &Q, StringStream &S, int level)
{
    StringStream indent;
    for (int N = 0; N < level; ++N)
    {
        indent.Append(' ');
        indent.Append(' ');
    }

    if (!Q.Valid())
        return;

    StorageBase const &base = Q.GetStorageBase();
    ClassBase const &klass = *Q.GetClass();
    S << indent.ToString() << "<Object type='" << klass.GetName()
        << "' name='" << base.GetLabel().ToString()
        //<< "' handle='" << (int)Q.GetHandle().GetValue()
        << "'>\n";

    if (Q.GetClass()->HasTraitsProperty(Type::Properties::StringStreamInsert))
    {
        S << indent.ToString() << "  <Value>";
        if (klass.HasOperation(Type::Properties::StringStreamInsert))
            klass.Insert(S, base);
        S << "</Value>\n";
    }

    for (auto const &prop_iter : klass.GetProperties())
    {
        PropertyBase const &property = *prop_iter.second;
        S << indent.ToString() <<"<Property name='" << property.GetFieldName() << "'>";
        //ToXmlStream(child.second, S, level + 1);
        S << property.GetValue(base);
        S << "</Property>\n";
    }

    const Dictionary &dict = base.GetDictionary();
    for (auto const &child : dict)
        ToXmlStream(child.second, S, level + 1);

    S << indent.ToString() << "</Object>\n";

    return;
}

KAI_END

