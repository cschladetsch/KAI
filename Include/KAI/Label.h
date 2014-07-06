

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_LABEL_H
#	define KAI_LABEL_H

KAI_BEGIN

struct Label
{
	typedef String Value;

private:
	Value value;

public:
	Label() { }
	Label(const String::Char *S) { FromString(S); }
	Label(const Value &S) { FromString(S); }

	bool Empty() const { return value.empty(); }

	void FromString(const Value &S);
	String ToString() const;

	void FromString2(Value S);
	const Value &GetValue() const { return value; }

	friend bool operator<(const Label &A, const Label &B) { return A.value < B.value; }
	friend bool operator==(const Label &A, const Label &B) { return A.value == B.value; }

	static void Register(Registry &);
};

StringStream &operator<<(StringStream &S, const Label &L);
BinaryStream &operator<<(BinaryStream &, Label const &);
BinaryPacket &operator>>(BinaryPacket &, Label &);

KAI_TYPE_TRAITS(Label, Type::Number::Label
	, Type::Properties::StringStreamInsert 
	| Type::Properties::Relational
	| Type::Properties::BinaryStreaming);

KAI_END

namespace boost
{
	inline size_t hash_value(KAI_NAMESPACE(Label) const &label) 
	{
		return hash_value(label.GetValue());
	}
}

#endif // KAI_LABEL_H

//EOF
