
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_LIST_H
#	define KAI_LIST_H

KAI_BEGIN

/// A list of Objects
struct List : Container<List>
{
	typedef nstd::list<Object> Objects;
	typedef Objects::const_iterator const_iterator;
	typedef Objects::iterator iterator;

private:
	Objects objects;

public:

	bool Destroy()
	{
		Clear();
		return true;
	}

	iterator begin() { return objects.begin(); }
	iterator end() { return objects.end(); }
	const_iterator begin() const { return objects.begin(); }
	const_iterator end() const { return objects.end(); }

	iterator Begin() { return objects.begin(); }
	iterator End() { return objects.end(); }
	const_iterator Begin() const { return objects.begin(); }
	const_iterator End() const { return objects.end(); }
	int Size() const { return (int)objects.size(); }
	bool Empty() const { return objects.empty(); }
	Object Front() const { return objects.front(); }
	Object Back() const { return objects.back(); }

	void Clear();

	void Append(Object const &Q);
	void PushBack(Object const &Q) { Append(Q); }
	Object Pop();
	Object PopBack() { return Pop(); }
	iterator Erase(iterator A);
	iterator Erase(Object const &Q);
	bool Contains(Object const &Q) const
	{
		foreach (Object const &element, objects)
		{
			if (element.GetHandle() == Q.GetHandle())
				return true;
		}
		return false;
	}

	void Erase2(Object Q) { Erase(Q); }
	void Append2(Object Q) { Append(Q); }
	bool Contains2(Object Q) const { return Contains(Q); }

	friend bool operator==(const List &A, const List &B) { return A.objects == B.objects; }
	friend bool operator<(const List &A, const List &B) { return A.objects < B.objects; }

	void SetChildSwitch(int N, bool M)
	{
		ForEach(objects, SetSwitch<List>(N, M));
	}

	static void Register(Registry &);
};

StringStream &operator<<(StringStream &, const List &);
BinaryStream &operator<<(BinaryStream &, const List &);
BinaryPacket &operator>>(BinaryPacket &, List &);

HashValue GetHash(const List &A);

KAI_TYPE_TRAITS(List, Type::Number::List
	, Type::Properties::StringStreamInsert 
	| Type::Properties::BinaryStreaming
	| Type::Properties::Less 
	| Type::Properties::Equiv
	| Type::Properties::Assign
	| Type::Properties::Reflected
	| Type::Properties::Container);

KAI_END

#endif // KAI_LIST_H

//EOF
