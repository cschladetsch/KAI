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
	StringToOperation::iterator A = string_to_op.begin(), B = string_to_op.end();
	for (; A != B; ++A)
		A->second.SetManaged(true);
	return true;
}

void Compiler::AddOperation(int N, const String &S)
{
	Operation T = Operation::Type(N);
	Pointer<Operation> Q = Self->New(T);
	Q.SetManaged(false);
	string_to_op[S] = Q;
	op_to_string[T] = S;
}



Pointer<Continuation> Compiler::Compile(Registry &R, const String &text, Parser::Structure st) const
{
	std::shared_ptr<RhoLang> p = std::make_shared<RhoLang>(R);

	p->Translate(text.c_str(), st);
	
	if (p->Failed)
	{
		KAI_TRACE_ERROR_1(p->Error);
		std::cerr << p->Error;
		return 0;
	}

	return p->trans->stack.back();
}

//Object Compiler::Compile(String const &text, Parser::Structure st) const
//{
//	return Compile(*Self->GetRegistry(), text, st);
//}
//
//Object Compiler::Compile2(String text) const
//{
//	return Compile(*Self->GetRegistry(), text, Parser::ParseExpression);
//}

void Compiler::Register(Registry &R, const char *name)
{
	//Object (Compiler::*Compile)(String) const = &Compiler::Compile2;
	ClassBuilder<Compiler>(R, name)
		.Methods
		//("Compile", Compile)
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

// TODO: use char for operation::value

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

//EOF
