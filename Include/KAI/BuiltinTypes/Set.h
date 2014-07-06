
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_BUILTIN_TYPES_SET_H
#	define KAI_BUILTIN_TYPES_SET_H

#	include "boost/unordered_set.hpp"

KAI_BEGIN

struct CompareHandles
{
	bool operator()(Object const &A, Object const &B) const
	{
		return A.GetHandle() == B.GetHandle();
	}
};

struct ObjectSet : Container<ObjectSet>
{
	typedef boost::unordered_set<Object, boost::hash<Object>, CompareHandles> Objects;
	typedef Objects::const_iterator const_iterator;
	typedef Objects::iterator iterator;

private:
	Objects objects;

public:

	bool Destroy();

	const_iterator begin() const { return objects.begin(); }
	const_iterator end() const { return objects.end(); }
	iterator begin() { return objects.begin(); }
	iterator end() { return objects.end(); }

	const_iterator Begin() const { return objects.begin(); }
	const_iterator End() const { return objects.end(); }
	iterator Begin() { return objects.begin(); }
	iterator End() { return objects.end(); }

	int Size() const;
	bool Empty() const;
	void Clear();
	iterator Erase(iterator);
	iterator Erase(Object const &);
	void Append(Object const &);
	void Insert(Object const &);
	void Remove(Object const &);
	bool Contains(Object const &);

	void SetChildSwitch(int N, bool M)
	{
		ForEach(objects, SetSwitch<ObjectSet>(N, M));
	}

	static void Register(Registry &);
};

KAI_TYPE_TRAITS(ObjectSet, Type::Number::Set
	, Type::Properties::StringStreamInsert 
	| Type::Properties::BinaryStreaming 
	| Type::Properties::Reflected
	| Type::Properties::Container);

StringStream &operator<<(StringStream &, ObjectSet const &);
StringStream &operator<<(StringStream &, ObjectSet::const_iterator const &);

BinaryStream &operator<<(BinaryStream &, ObjectSet const &);
BinaryStream &operator>>(BinaryStream &, ObjectSet &);

KAI_END

#endif // KAI_BUILTIN_TYPES_SET_H

//EOF

