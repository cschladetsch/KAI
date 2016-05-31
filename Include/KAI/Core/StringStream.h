#pragma once

#include <vector>

#include <KAI/Core/Config/Base.h>
#include <KAI/Core/BuiltinTypes/String.h>

KAI_BEGIN

struct Trace;

class StringStream
{
public:
	typedef String::Char Char;
	typedef std::vector<Char> Storage;

private:
	Storage stream;
	int read_offset;
	Registry *registry;

public:
	StringStream() : read_offset(0), registry(0) { }
	explicit StringStream(String const &S) : read_offset(0), registry(0) { Append(S); }

	String ToString() const;
	bool Empty() const { return stream.empty(); }
	int Size() const { return (int)stream.size(); }
	void Clear() { stream.clear(); }
	bool CanRead(int = 1) const;

	void SetRegistry(Registry *R) { registry = R; }
	void Append(Char);
	void Append(const Char *);
	void Append(const Char *A, const Char *B);
	void Append(const String &);

	bool Extract(int length, String &);
	bool Extract(Char &);
	char Peek() const;

	static void Register(Registry &);
};

struct EndsArgument { };
void Ends(EndsArgument);

StringStream &operator<<(StringStream &, void (*)(EndsArgument));
StringStream &operator<<(StringStream &, const String::Char *);
//StringStream &operator<<(StringStream &, const String::Char);

inline StringStream &operator<<(StringStream &S, const String &T) { return S << T.c_str(); }
//inline StringStream &operator<<(StringStream &S, const Exception::Base &E) { return S << E.ToString().c_str(); }
//
//StringStream& operator<<(StringStream&, ObjectColor::Color);
//
//KAI_TYPE_TRAITS(
//	StringStream,
//	Number::StringStream,
//	Properties::Equiv);

KAI_END
