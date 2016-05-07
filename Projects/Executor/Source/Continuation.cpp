
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
		return false;

	next = code->At(n++);

	return true;
}

const static int MaxDepth = 3;
const static int MaxLen = 80;

StringStream &InsertContinuation(StringStream &stream, const Array &code, size_t index, int depth)
{
	// too long
	if (stream.Size() >= MaxLen)
		return stream << "...}";

	// finished
	if (index == code.Size())
		return stream << "}";

	auto const &next = code.At(index);

	// print next thing
	if (next.IsType<Continuation>())
	{
		// limit depth of coro pritning
		if (depth == MaxDepth)
			return stream << "{...}...";

		auto const &coro = ConstDeref<Continuation>(next);
		return InsertContinuation(stream, *coro.GetCode(), 0, ++depth);
	}

	while (code.Size() > index)
	{
		stream << code.At(index) << " ";
		InsertContinuation(stream, code, ++index, depth);
	}

	return stream;
}

StringStream &operator<<(StringStream &S, const Continuation &C)
{
	return InsertContinuation(S << "{ ", *C.GetCode(), 0, 0);
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
