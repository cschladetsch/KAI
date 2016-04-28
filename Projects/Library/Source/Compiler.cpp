#include "KAI/KAI.h"
#include "KAI/Operation.h"
#include "KAI/Value.h"
#include "KAI/Continuation.h"
#include "KAI/Compiler.h"
#include "KAI/Reflected.h"
#include "KAI/Executor.h"
#include "KAI/Pathname.h"

#include <iostream>
#include <algorithm>
#include <assert.h>

KAI_BEGIN

#include "CompilerTokens.inl"

bool Compiler::Destroy()
{
	// these are unmanaged while the compiler is alive, to avoid them being part
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

Pointer<Continuation> Compiler::Compile(Registry &R, const String &text, Parser::Structure st) const
{
	if (text.empty())
		return Object();

	std::shared_ptr<RhoLang> p = std::make_shared<RhoLang>(R);
	p->Translate(text.c_str(), st);
	if (p->Failed)
	{
		std::cerr << p->Error;
		return Object();
	}

	return p->trans->stack.back();
}

Pointer<Continuation> Compiler::CompileFile(Registry &R, const String &fileName, Parser::Structure st) const
{
	std::shared_ptr<RhoLang> p = std::make_shared<RhoLang>(R);
	p->TranslateFile(fileName.c_str(), st);
	if (p->Failed)
	{
		KAI_TRACE_ERROR_1(p->Error);
		std::cerr << p->Error;
		return Object();
	}

	return p->trans->stack.back();
}

void Compiler::Register(Registry &R, const char *name)
{
	ClassBuilder<Compiler>(R, name)
		.Methods
		;
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

#include "OperationToString.inl"

const char *Operation::ToString() const
{
	return ToString((int)value);
}

void Operation::Register(Registry &R)
{
	ClassBuilder<Operation>(R, "Operation")
		;
}

KAI_END

