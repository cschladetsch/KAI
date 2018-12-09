#include <iostream>
#include <fstream>

#include <KAI/Core/BuiltinTypes.h>
#include <KAI/Executor/Compiler.h>
#include <KAI/Language/Rho/RhoTranslator.h>
#include <KAI/Language/Pi/PiTranslator.h>
#include <KAI/Core/File.h>

using namespace std;

KAI_BEGIN

#include "CompilerTokens.inl"

bool Compiler::Destroy()
{
    // these are un-managed while the compiler is alive, to avoid them being part
    // of the GC process. probably an overkill for performance. in any case,
    // when the compiler goes out of scope, we want to ensure the operation objects
    // are returned to the pool for GC.
    // again, all this work and text describing it is probably more cost than just
    // letting op's be managed from the start.
    for (auto op : string_to_op)
        op.second.SetManaged(true);

    return true;
}

Pointer<Continuation> Compiler::Translate(const String &text, Structure st) const
{
    switch (_language)
    {
    case Language::None:
        return Object();

    case Language::Pi:
        {
            // Pi wraps output (A continuation) in a continuation. So the output has to be
            // dereferenced once.
            //
            // TODO: not have to do this.
            auto top = Compile<PiTranslator>(text, st);
            if (!top.Exists())
                return Object();
            auto inner = top->GetCode();
            if (!inner.Exists())
                return Object();
            return inner->At(0);
        }

    case Language::Rho:
        return Compile<RhoTranslator>(text, st);

    case Language::Tau:
        KAI_NOT_IMPLEMENTED();    // Tau is not a compiled language - it is an IDL
        break;
    }

    return Object();
}

void Compiler::AddOperation(int N, const String &S)
{
    Operation T = Operation::Type(N);
    Pointer<Operation> Q = Self->GetRegistry()->New(T);
    Q.SetManaged(false);
    string_to_op[S] = Q;
    op_to_string[T] = S;
}

Pointer<Continuation> Compiler::CompileFile(const String &fileName, Structure st) const
{
    auto text = ReadTextFile(fileName.c_str());
    return Translate(text.c_str(), st);
}

void Compiler::Register(Registry &R, const char *name)
{
    ClassBuilder<Compiler>(R, name)
        ;
}

void Compiler::SetLanguage(int n)
{
    _language = static_cast<Language>(n);
}

int Compiler::GetLanguage() const
{
    return static_cast<int>(_language);
}

StringStream &operator<<(StringStream &S, Operation const &P)
{
    return S << P.ToString();
}

StringStream &operator>>(StringStream &, Operation &)
{
    KAI_NOT_IMPLEMENTED();
}

BinaryStream &operator<<(BinaryStream &S, Operation const &P)
{
    return S << (int)(P.GetTypeNumber());
}

BinaryStream &operator>>(BinaryStream &S, Operation &P)
{
    int val;
    S >> val;
    P.SetType(Operation::Type(val));
    return S;
}

//const char *Operation::ToString() const
//{
//    return ToString((int)value);
//}
//
void Operation::Register(Registry &R)
{
    ClassBuilder<Operation>(R, "Operation")
        ;
}

KAI_END

