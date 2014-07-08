
#include "KAI/KAI.h"
#include "KAI/Operation.h"
#include "KAI/BuiltinTypes/Array.h"
#include "KAI/Continuation.h"
#include "KAI/Executor.h"

KAI_BEGIN

void Continuation::Create()
{
	entered = false;
	args = New<Array>();
	index = New<int>(0);
}

bool Continuation::Destroy()
{
	return true;
}

void Continuation::SetCode(Code C)
{
	code = C;
	*entered = false;
}

void Continuation::Enter(Executor *exec)
{
	if (*entered)
		return;

	if (code.Exists() && !code->Empty())
	{
		if (!scope.Exists())
			scope = exec->New<void>();

		for (auto arg : *args)
		{
			scope.Set(ConstDeref<Label>(arg), exec->GetDataStack()->Pop());
		}

		*index = 0;
		*entered = true;
	}
}

bool Continuation::Next() const
{
	Object unused;
	return Next(unused);
}

bool Continuation::Next(Object &next) const
{ 
	if (!code)
		return false;
	if (!entered)
		KAI_THROW_1(LogicError, "Continuation not Entered");

	int &n = Deref<int>(index);
	if (n == code->Size())
		return false;

	next = code->At(n++);
	return true;
}

StringStream &operator<<(StringStream &S, const Continuation &C)
{
	S << "{ ";
	if (C.GetCode().Exists())
	{
		Array::const_iterator A = C.GetCode()->Begin(), B = C.GetCode()->End();
		for (; A != B; ++A)
			S << *A << " ";
	}
	S << "}";
	//S << "args(";
	//for (auto a : C.GetArgs())
	//	S << a << ' ';
	//S << ")";
	return S;
}

StringStream &operator>>(StringStream &, Continuation &)
{
	KAI_NOT_IMPLEMENTED();
}

BinaryStream &operator<<(BinaryStream &S, const Continuation &C)
{
	// TODO: instruction pointer and scope!
	return S << C.GetCode();
}

BinaryPacket &operator>>(BinaryPacket &S, Continuation &C)
{
	Object code;
	// TODO: instruction pointer and scope!
	S >> code;
	C.SetCode(code);
	return S;
}

void Continuation::Register(Registry &R)
{
	ClassBuilder<Continuation>(R, "Continuation")
		.Methods
		.Properties
		("code", &Continuation::code)
		("args", &Continuation::args)
		("scope", &Continuation::scope)
		("source_code", &Continuation::source_code)
		("entered", &Continuation::entered)
		;
}

KAI_END

//EOF
