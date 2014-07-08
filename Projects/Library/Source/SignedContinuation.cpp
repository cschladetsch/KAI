
#include "KAI/KAI.h"
#include "KAI/Operation.h"
#include "KAI/Continuation.h"
#include "KAI/Executor.h"
#include "KAI/SignedContinuation.h"
#include "KAI/Tree.h"

#include <iostream>

KAI_BEGIN

void SignedContinuation::Create(Pointer<Array> args, Pointer<Array> rtypes, Pointer<Continuation> C, Pointer<Executor> exec)
{
	cont = C;

	if (args.Exists())
	{
		if ((args->Size() & 1) != 0)
			KAI_THROW_1(Base, "SignedContinuation argument list must be a list of (type, label)");
		cont = C;

		// add the formal params
		Array::const_iterator A = args->Begin(), B = args->End();
		while (A != B)
		{
			Object type_ident = *A++;
			Pointer<Label> label = *A++;
			Pointer<const ClassBase *> klass = exec->Resolve(type_ident);
			params.push_back(FormalParameter((*klass)->GetTypeNumber(), *label));
		}
	}

	// set the return types
	if (rtypes.Exists())
	{
		// TODO
	}
}

void SignedContinuation::Enter(Stack &stack)
{
	KAI_UNUSED_1(stack);

//	// reset the continuation
//	cont->Enter();
//
//	// ensure we have enough args
//	if (stack.Size() < (int)params.size())
//		KAI_THROW_0(EmptyStack);
//
//	// check argument types
//	FormalParameters::const_reverse_iterator A = params.rbegin(), B = params.rend();
//	for (int N = 0; A != B; ++A, ++N)
//	{
//		Object param = stack.At(N);
//		//if (!param.IsTypeNumber(A->type))
//		//	KAI_THROW_2(TypeMismatch, (int)param.GetTypeNumber().GetValue(), (int)A->type.GetValue());
//	}
//
//	// make a new scope
//	cont->SetScope(cont.New<void>());
//	
////	std::cout << "set cont scope=" << cont->GetScope().GetHandle().GetValue() << std::endl;
//
//	// store the arguments
//	A = params.rbegin();
//	for (; A != B; ++A)
//	{
//		cont->GetScope().Set(A->label, stack.Pop());
//	}
}

void SignedContinuation::Leave(Stack &)
{
	// TODO check the return types
}

StringStream &operator<<(StringStream &S, SignedContinuation const &Q)
{
	S << "SignedContinuation: ";
	String sep = "";
	SignedContinuation::FormalParameters::const_iterator A = Q.params.begin(), B = Q.params.end();
	for (; A != B; ++A)
	{
		S << sep << A->type << " " << A->label;
		sep = ", ";
	}
	S << " -> ";

	sep = "";
	SignedContinuation::ReturnTuple::const_iterator C = Q.return_tuple.begin(), D = Q.return_tuple.end();
	for (; C != D; ++C)
	{
		S << sep << C->GetValue();
		sep = ", ";
	}
	return S << "\nContinuation: " << Q.cont;
}

void SignedContinuation::Register(Registry &R)
{
	ClassBuilder<SignedContinuation>(R, "SignedContinuation")
		;
}

KAI_END

//EOF
