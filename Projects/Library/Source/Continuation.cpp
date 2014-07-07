
#include "KAI/KAI.h"
#include "KAI/Operation.h"
#include "KAI/BuiltinTypes/Array.h"
#include "KAI/Continuation.h"

KAI_BEGIN

Continuation::Continuation()
{
	source_code = 0;
	Entered = false;
}

void Continuation::SetCode(Code C)
{
	code = C;
	Entered = false;
	Enter();
}

void Continuation::Enter() const
{
	if (code.Exists() && !code->Empty())
	{
		Entered = true;
		ip = code->Begin();
	}
}

bool Continuation::Next() const
{
	if (!HasCode())
		return false;
	if (!Entered)
		Enter();
	if (ip == code->end())
		return false;
	++ip;
	return true;
}

bool Continuation::Next(Object const *&next) const
{ 
	if (!code)
		return false;
	if (!Entered)
		Enter();
	if (ip == code->end())
		return false;
	next = &*ip++;
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
	S << "args(";
	for (auto a : C.GetArgs())
		S << a << ' ';
	S << ")";
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
		("scope", &Continuation::scope)
		;
}

KAI_END

//EOF
