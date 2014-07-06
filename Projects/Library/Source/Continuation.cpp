
#include "KAI/KAI.h"
#include "KAI/Operation.h"
#include "KAI/BuiltinTypes/Array.h"
#include "KAI/Continuation.h"

KAI_BEGIN

void Continuation::Enter()
{
	if (code.Exists())
		ip = code->Begin();
}

Continuation::InstructionPointer &Continuation::Ip() 
{ 
	return ip; 
}

Continuation::InstructionPointer Continuation::End() const 
{ 
	//auto c = *code;
	//return c.End(); 
	return code->End();
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
