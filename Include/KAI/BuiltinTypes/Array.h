#pragma once

KAI_BEGIN

struct Array;

namespace Type
{
	template <>
	struct Traits<Array> : TraitsBase<Array, Number::Array
		, Properties::StringStreamInsert
		| Properties::BinaryStreaming
		| Properties::Less
		| Properties::Equiv
		| Properties::Assign
		| Properties::Reflected
		| Properties::Container>
	{
		static const char *Name;
	};

	const char * Traits<Array>::Name = "Array";
}

/*
KAI_TYPE_TRAITS(Array, Type::Number::Array
	, Type::Properties::StringStreamInsert 
	| Type::Properties::BinaryStreaming
	| Type::Properties::Less 
	| Type::Properties::Equiv
	| Type::Properties::Assign
	| Type::Properties::Reflected
	| Type::Properties::Container
	);
*/

/// A vector of Objects
struct Array : Container<Array>
{
	typedef nstd::vector<Object> Objects;
	typedef Objects::const_iterator const_iterator;
	typedef Objects::iterator iterator;

private:
	Objects objects;

public:
	iterator begin() { return objects.begin(); }
	iterator end() { return objects.end(); }
	const_iterator begin() const { return objects.begin(); }
	const_iterator end() const { return objects.end(); }

	iterator Begin() { return objects.begin(); }
	iterator End() { return objects.end(); }
	const_iterator Begin() const { return objects.begin(); }
	const_iterator End() const { return objects.end(); }

	Object At(int pos) const { return objects.at(pos); }
	int Size() const { return (int)objects.size(); }
	bool Empty() const { return objects.empty(); }
	Object Front() const { return objects.front(); }
	Object Back() const { return objects.back(); }

	void Clear();
	void Append(Object const &);
	void PushBack(Object const &Q) { Append(Q); }
	Object PopBack();
	void RemoveAt(int);

	iterator Erase(iterator A);
	void Erase(Handle A);
	void Erase(Object const &Q);

	void Erase2(Object Q) { Erase(Q); }
	void Append2(Object Q) { Append(Q); }

	friend bool operator==(const Array &A, const Array &B) { return A.objects == B.objects; }
	friend bool operator<(const Array &A, const Array &B) { return A.objects < B.objects; }

	//void SetChildSwitch(int N, bool M)
	//{
	//	ForEach(objects, SetSwitch<Array>(N, M));
	//}

	static void Register(Registry &);
};

StringStream &operator<<(StringStream &, const Array &);
BinaryStream &operator<<(BinaryStream &, const Array &);
BinaryPacket &operator>>(BinaryPacket &, Array &);

HashValue GetHash(const Array &A);

KAI_END
