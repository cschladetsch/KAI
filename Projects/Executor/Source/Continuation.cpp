#include "KAI/ExecutorPCH.h"

KAI_BEGIN

void Continuation::Create()
{
	entered = New(false);
	scopeBreak = New(false);
	args = New<Array>();
	index = New(0);
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

		Stack &data = *exec->GetDataStack();
		if (data.Size() < args->Size())
		{
			KAI_TRACE_ERROR_2(data.Size(), args->Size()) << "Failed to enter continuation: not enough args";
			KAI_THROW_0(EmptyStack);
		}

		for (auto arg : *args)
		{
			Object a = data.Pop();
			scope.Set(ConstDeref<Label>(arg), a);
		}
	}

	*index = 0;
	*entered = true;
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

	if (!*entered)
		KAI_THROW_1(LogicError, "Continuation not Entered");

	int &n = Deref<int>(index);
	if (n == code->Size())
		return n != 0;

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

	S << "}" << "@" << *C.index;

	if (C.code->Size() > 0)
	{
		S << ":: ";
		int n = *C.index;

		if (n < C.code->Size())
			S << C.code->At(n);
		else
			S << "[end]";
	}

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
		// TODO ("code", &Continuation::code)
		//("args", &Continuation::args)
		//("scope", &Continuation::scope)
		//("source_code", &Continuation::source_code)
		//("entered", &Continuation::entered)
		;
}

KAI_END
