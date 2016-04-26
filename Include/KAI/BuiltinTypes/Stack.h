
#pragma once

KAI_BEGIN

class Stack;

KAI_TYPE_TRAITS(Stack, Number::Stack
	, Properties::StringStreamInsert 
	| Properties::BinaryStreaming
	| Properties::Less 
	| Properties::Equiv
	| Properties::Assign
	| Properties::Reflected
	| Properties::Container
	);

class Stack : public Container<Stack>
{
public:
	typedef std::vector<Object> Objects;
	typedef Objects::const_iterator const_iterator;
	typedef Objects::iterator iterator;

private:
	Objects stack;
	
public:
	friend bool operator==(const Stack &A, const Stack &B) { return A.stack == B.stack; }
	friend bool operator<(const Stack &A, const Stack &B) { return A.stack < B.stack; }

	bool Destroy();

	iterator Begin() { return stack.begin(); }
	iterator End() { return stack.end(); }
	const_iterator Begin() const { return stack.begin(); }
	const_iterator End() const { return stack.end(); }
	bool Empty() const { return stack.empty(); }
	int Size() const { return (int)stack.size(); }

	Object At(int N) const;
	void Push(Object const &Q);
	void Clear();
	iterator Erase(Object const &);
	iterator Erase(iterator);
	Object Pop();
	Object Top() const;

	static void Register(Registry &);

	iterator begin() { return stack.begin(); }
	iterator end() { return stack.end(); }
	const_iterator begin() const { return stack.begin(); }
	const_iterator end() const { return stack.end(); }
};

StringStream &operator<<(StringStream &, const Stack &);
BinaryStream &operator<<(BinaryStream &, const Stack &);
BinaryPacket &operator>>(BinaryPacket &, Stack &);

HashValue GetHash(const Stack &);

KAI_END
