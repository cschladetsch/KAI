#include "KAI/ExecutorPCH.h"

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
		S << sep << A->type.TypeNumber << String(" ") << A->label;
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
