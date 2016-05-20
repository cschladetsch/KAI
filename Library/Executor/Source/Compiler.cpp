#include "KAI/Core/Object.h"
#include "KAI/Core/Compiler.h"
#include "KAI/Core/ClassBuilder.h"

#include <iostream>
#include <algorithm>
#include <assert.h>

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

void Compiler::AddOperation(int N, const String &S)
{
	Operation T = Operation::Type(N);
	Pointer<Operation> Q = Self->GetRegistry()->New(T);
	Q.SetManaged(false);
	string_to_op[S] = Q;
	op_to_string[T] = S;
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

BinaryPacket &operator>>(BinaryPacket &S, Operation &P)
{
	int val;
	S >> val;
	P.SetType(Operation::Type(val));
	return S;
}

//const char *Operation::ToString() const
//{
//	return ToString((int)value);
//}
//
void Operation::Register(Registry &R)
{
	ClassBuilder<Operation>(R, "Operation")
		;
}

KAI_END

