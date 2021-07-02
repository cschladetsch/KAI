#include "KAI/Core/BuiltinTypes.h"
#include "KAI/Core/Exception.h"
#include "KAI/Core/FunctionBase.h"
#include "rang.hpp"

KAI_BEGIN

StringStream &operator >> (StringStream &S, float &F)
{
    //return S >> F;
    KAI_NOT_IMPLEMENTED();
}

/* TODO WTF
StringStream &operator<<(StringStream &S, const BasePointerBase &B)
{
    B.Self->GetClass()->Insert(S, *B.Self);
    return S;
}
*/

void BasePointerBase::Register(Registry &R)
{
    ClassBuilder<BasePointerBase>(R, Label("BasePointerBase"))
        ;
}

void Handle::Register(Registry &R)
{
    ClassBuilder<Handle>(R, Label("Handle"))
        .Methods
        ("GetValue", &Handle::GetValue)
        ;
}

void Pair::Register(Registry &R)
{
    ClassBuilder<Pair>(R, "Pair")
        .Methods
        .Properties
        ("first", &Pair::first)
        ("second", &Pair::second)
        ;
}

void FileLocation::AddLocation(StringStream &str) const
{
    if (debug::Trace::TraceFileLocation)
    {
        std::string loc = file.c_str();
        if (debug::Trace::StripPath)
        {
            loc = loc.substr(loc.find_last_of('/') + 1);
            loc = loc.substr(loc.find_last_of('\\') + 1);
        }

        if (!loc.empty())
    #ifdef __MSVC__
            S << loc.c_str() << "(" << line << "): ";
    #else
            str << loc.c_str() << ":" << line << ": ";
    #endif
    }
}

void FileLocation::AddFunction(StringStream &str) const
{
    if (debug::Trace::TraceFunction && !function.Empty())
        str << function << ": ";
}

String FileLocation::ToString() const
{
    StringStream str;
    AddLocation(str);
    AddFunction(str);
    str << Ends;
    return str.ToString();
}
 
template <class Callable>
void WriteArgumentList(StringStream &S, const typename Callable::ArgumentTypes &arguments)
{
    S << "(";
    String sep = "";
    for (const auto &A : arguments)
    {
        S << sep << A.ToString();
        sep = ", ";
    }

    S << ")";
}

String MethodBase::ToString() const
{
    StringStream S;
    S << return_type.ToString() << String(" ") << class_type.ToString() << String("::") << name;
    WriteArgumentList<MethodBase>(S, arguments);

    if (constness == Constness::Const)
        S << " const;";
    else
        S << ";";

    if (!Description.Empty())
        S << " /* " << Description << " */";

    return S.ToString();
}

String FunctionBase::ToString() const
{
    StringStream S;
    S << return_type.ToString() << " " << name;
    WriteArgumentList<FunctionBase>(S, arguments);
    S << ";";

    if (!Description.Empty())
        S << " /* " << Description << " */";

    return S.ToString();
}

void FunctionBase::Register(Registry &R)
{
    ClassBuilder<BasePointer<FunctionBase> >(R, "Function")
        ;
}

StringStream &operator<<(StringStream &S, int N)
{
    S << std::to_string(N);
    return S;
}

StringStream &operator>>(StringStream &S, int &N)
{
    String T;
    S >> T;
    N = atoi(T.c_str());
    return S;
}

StringStream &operator<<(StringStream &S, bool B)
{
    if (B)
        S << "true";
    else
        S << "false";

    return S;
}

StringStream &operator<<(StringStream &S, double N)
{
    KAI_UNUSED(N);
    return S;
}

// 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead.
#pragma warning(disable:4996)

StringStream &operator<<(StringStream &S, float N)
{
    char buffer[1000];
    sprintf(buffer, "%f", N);
    return S << buffer;
}

HashValue GetHash(const Pair &P)
{
    return GetHash(P.first);
}

StringStream &operator<<(StringStream &S, Type::Number T)
{
    return S << T.ToString();//"TypeNumber: " << T.ToInt() << "(" << T.ToString() << ")";
}

StringStream &operator<<(StringStream &S, BasePointer<PropertyBase> const &P)
{
    return S << "Property: _name=" << P->GetFieldName() << ", type=" << P->GetFieldTypeNumber().value << ", class=" << P->GetClassTypeNumber().value;
}

StringStream &operator<<(StringStream &S, Vector2 const &V)
{
    return S << V.x << " " << V.y;
}

StringStream &operator>>(StringStream &S, Vector2 &V)
{
    KAI_UNUSED_2(S,V);
    KAI_NOT_IMPLEMENTED();
}

BinaryStream &operator<<(BinaryStream &S, Vector2 const &V)
{
    return S << V.x << V.y;
}

BinaryStream &operator>>(BinaryStream &S, Vector2 &V)
{
    return S >> V.x >> V.y;
}


StringStream &operator<<(StringStream &S, Vector3 const &V)
{
    return S << V.x << " " << V.y << " " << V.z;
}

StringStream &operator>>(StringStream &S, Vector3 &V)
{
    return S >> V.x >> V.y >> V.z;
}

BinaryStream &operator<<(BinaryStream &S, Vector3 const &V)
{
    return S << V.x << V.y << V.z;
}

BinaryStream &operator>>(BinaryStream &S, Vector3 &V)
{
    return S >> V.x >> V.y >> V.z;
}

StringStream &operator<<(StringStream &S, Vector4 const &V)
{
    return S << V.x << " " << V.y << " " << V.x << " " << V.w;
}

StringStream &operator>>(StringStream &, Vector4 &)
{
    KAI_NOT_IMPLEMENTED();
}

BinaryStream &operator<<(BinaryStream &S, Vector4 const &V)
{
    return S << V.x << V.y << V.z << V.w;
}

BinaryStream &operator>>(BinaryStream &S, Vector4 &V)
{
    return S >> V.x >> V.y >> V.z >> V.w;
}

StringStream &operator<<(StringStream &S, std::size_t N)
{
    return S << (int)N;
}

StringStream &operator<<(StringStream &S, Handle H)
{
    return S << "#" << H.GetValue();
}

StringStream &operator<<(StringStream &S, Pair const &P)
{
    return S << "Pair(" << P.first << ", " << P.second << ")";
}

StringStream &operator>>(StringStream &S, Pair &P)
{
    KAI_UNUSED_2(S,P);
    KAI_NOT_IMPLEMENTED();
}

BinaryStream &operator<<(BinaryStream &S, Pair const &P)
{
    return S << P.first << P.second;
}

BinaryStream &operator>>(BinaryStream &S, Pair &P)
{
    return S >> P.first >> P.second;
}


KAI_END

//EOF
