#include <KAI/Core/ClassBuilder.h>

#include "KAI/Core/BuiltinTypes/Stack.h"
#include "KAI/Core/StringStream.h"
#include "KAI/Core/BinaryStream.h"

KAI_BEGIN

void Stack::Register(Registry &R)
{
	ClassBuilder<Stack>(R, "Stack")
		.Methods
		("Pop", &Stack::Pop)
		("Top", &Stack::Top)
		("Size", &Stack::Size)
		("Empty", &Stack::Empty)
		;
}

bool Stack::Destroy()
{
	Clear();
	return true;
}

Object Stack::At(int N) const
{
	if (N >= Size())
		KAI_THROW_0(EmptyStack);
	return stack[Size() - 1 - N];
}

Object Stack::Top() const
{
	if (stack.empty())
		KAI_THROW_0(EmptyStack);
	return stack.back();
}

void Stack::Push(Object const &Q) 
{ 
	stack.push_back(Q);
}

void Stack::Clear() 
{ 
	while (!Empty())
		Pop();
}

Stack::iterator Stack::Erase(Object const &Q)
{
	iterator A = std::find(begin(), end(), Q);
	if (A != end())
		return Erase(A);
	KAI_THROW_1(UnknownObject, Q.GetHandle());
}

Stack::iterator Stack::Erase(iterator A)
{
	Detach(*A);
	return stack.erase(A);
}

Object Stack::Pop()
{
	if (stack.empty())
		KAI_THROW_0(EmptyStack);
	Object Q = Top();
	Detach(Q);
	stack.pop_back();
	return Q;
}

StringStream &operator<<(StringStream &stream, const Stack &stack)
{
	stream << "Stack[";
	String sep;
	for (auto obj : stack)
	{
		stream << sep << obj;
		if (sep.empty())
			sep = ", ";
	}
	return stream << "]";
}

BinaryStream &operator<<(BinaryStream &S, const Stack &T)
{
	KAI_UNUSED_2(S,T);
	KAI_NOT_IMPLEMENTED();
}

BinaryPacket &operator>>(BinaryPacket &S, Stack &T)
{
	KAI_UNUSED_2(S,T);
	KAI_NOT_IMPLEMENTED();
}

KAI_END

