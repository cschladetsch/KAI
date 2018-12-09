#include "KAI/Core/BasePointer.h"
#include "KAI/Core/StringStream.h"
#include "KAI/Core/BinaryPacket.h"
#include "KAI/Core/BinaryStream.h"
#include "KAI/Core/FunctionBase.h"
#include <KAI/Executor/Operation.h>
#include <KAI/Core/Object/ClassBuilder.h>

KAI_BEGIN

void StringStream::Register(Registry &registry)
{
    KAI_UNUSED(registry);
//TODO
//    ClassBuilder<StringStream>(registry, "StringStream")
//        .Methods
//            ("Size", &StringStream::Size)
//            ("Empty", &StringStream::Empty)
//            ("ToString", &StringStream::ToString)
//            ("Clear", &StringStream::Clear)
////TODO        ("Append", &StringStream::Append)
//        .Properties
//    ;
}

bool StringStream::CanRead(int N) const
{
    return read_offset + N < (int)stream.size();
}

StringStream &operator<<(StringStream &S, const BinaryPacket &T)
{
    return S << "BinaryPacket: size=" << T.Size();
}

StringStream &operator<<(StringStream &S, const BinaryStream &T)
{
    return S << "BinaryStream: size=" << T.Size();
}

StringStream &operator<<(StringStream &S, const BasePointerBase &M)
{
    KAI_NOT_IMPLEMENTED();
    return S;
}

StringStream &operator<<(StringStream &S, const BasePointer<MethodBase> &M)
{
    return S << "Method: " << M->ToString();
}

StringStream &operator<<(StringStream &S, const BasePointer<FunctionBase> &F)
{
    return S << "Function: " << F->ToString();
}

String StringStream::ToString() const
{
    return String(stream.begin(), stream.end());
}

void StringStream::Append(Char C)
{
    stream.push_back(C);
}

void StringStream::Append(const Char *C)
{
    Append(C, C + strlen(C));
}

void StringStream::Append(const Char *A, const Char *B)
{
    std::copy(A, B, std::back_inserter(stream));
}

void StringStream::Append(const String &S)
{
    //Append(S.begin(), S.end());
    if (!S.empty())
        Append(S.c_str());//[0], &S[S.size() - 1]);
}

bool StringStream::Extract(int, String &)
{
    KAI_NOT_IMPLEMENTED();
}

char StringStream::Peek() const
{
    if (!CanRead(1))
        return 0;
    return stream[read_offset];
}

bool StringStream::Extract(Char &C)
{
    if (read_offset > (int)stream.size() - 1)
        return false;

    C = stream[read_offset++];
    return true;
}

StringStream &operator<<(StringStream &S, void(*)(EndsArgument))
{
    //S.Append('\0');
    return S;
}

StringStream &operator<<(StringStream &S, const String::Char *P)
{
    S.Append(P);
    return S;
}

StringStream &operator<<(StringStream &S, const String::Char C)
{
    S.Append(C);
    return S;
}


StringStream &operator<<(StringStream &stream, const Object &object)
{
    if (!object.Exists())
        return stream << "Null";

    const ClassBase *klass = object.GetClass();
    if (klass == 0)
        return stream << "Classless";

    if (klass->HasOperation(Type::Properties::StringStreamInsert))
        klass->Insert(stream, object.GetStorageBase());
    else
    {
        if (klass->GetTypeNumber() == Type::Number::Operation)
        {
            //Operation op = ConstDeref<Operation>(object);
            //stream << "Op: " << op.GetTypeNumber();
            stream << Operation::ToString(ConstDeref<Operation>(object).GetTypeNumber());
        }
        else
        {
            stream << "Handle=" << object.GetHandle().GetValue() << ", type=" << klass->GetName() << " ";
        }
    }

    return stream;
}

StringStream &operator<<(StringStream &S, const ClassBase *C)
{
    if (C == 0)
        return S << "NullClass";

    return S << "Class: " << C->GetName();
}

StringStream &operator>>(StringStream &S, bool &N)
{
    String value;
    S >> value;
    N = value == "true";
    return S;
}

void Ends(EndsArgument)
{
    KAI_NOT_IMPLEMENTED_1("This is used to terminate string streams. Do not call it.");
}

KAI_END

