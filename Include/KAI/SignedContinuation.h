
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_SIGNED_CONTINUATION
#	define KAI_SIGNED_CONTINUATION

KAI_BEGIN

struct SignedContinuation
{
	struct FormalParameter
	{
		Type::Number type;
		Label label;	// pass by ref if quoted
		FormalParameter() { }
		FormalParameter(Type::Number T, Label const &L) : type(T), label(L) { }
	};

	typedef nstd::vector<FormalParameter> FormalParameters;
	typedef nstd::vector<Type::Number> ReturnTuple;

	Pointer<Continuation> cont;
	FormalParameters params;
	ReturnTuple return_tuple;

	void Create(Pointer<Array> args, Pointer<Array> rtypes, Pointer<Continuation> cont, Pointer<Executor> exec);
	Object GetContinuation() const { return cont; }
	void Enter(Stack &);
	void Leave(Stack &);
	static void Register(Registry &R);
};

StringStream &operator<<(StringStream &, SignedContinuation const &);

KAI_TYPE_TRAITS(SignedContinuation, Type::Number::SignedContinuation, Type::Properties::StringStreamInsert);

KAI_END

#endif	// KAI_SIGNED_CONTINUATION

//EOF
