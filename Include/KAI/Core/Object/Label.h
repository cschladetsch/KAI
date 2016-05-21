#pragma once

#include <KAI/Core/BuiltinTypes/String.h>
#include <KAI/Core/Type/TraitMacros.h>

KAI_BEGIN

struct Label
{
	typedef String Value;

private:
	Value value;

public:
	Label() { }
	explicit Label(const String::Char *S) { FromString(S); }
	Label(const Value &S) { FromString(S); }

	bool Empty() const { return value.empty(); }

	void FromString(const Value &S);
	String ToString() const;

	void FromString2(Value S);
	const Value &GetValue() const { return value; }

	friend bool operator<(const Label &A, const Label &B) 
	{
	   	return A.value < B.value; 
	}

	friend bool operator==(const Label &A, const Label &B) 
	{
	   	return A.value == B.value; 
	}

	static void Register(Registry &);
};

StringStream &operator<<(StringStream &S, const Label &L);
BinaryStream &operator<<(BinaryStream &, Label const &);
BinaryStream &operator>>(BinaryStream &, Label &);

KAI_TYPE_TRAITS(Label, Number::Label
	, Properties::StringStreamInsert 
	| Properties::Relational
	| Properties::BinaryStreaming);

KAI_END

//#include "KAI/Core/LabelHash.h"

namespace boost
{
	inline size_t hash_value(KAI_NAMESPACE(Label) const &label) 
	{
		return hash_value(label.GetValue());
	}
}

