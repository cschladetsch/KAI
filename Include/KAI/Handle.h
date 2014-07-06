

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_HANDLE_H
#	define KAI_HANDLE_H

KAI_BEGIN

struct Handle
{
	typedef int Value;

private:
	Value value;

public:
	Handle(Value V = 0) : value(V) { }

	Value GetValue() const { return value; }
	Value NextValue() { return ++value; }

	friend bool operator<(Handle A, Handle B) { return A.value < B.value; }
	friend bool operator==(Handle A, Handle B) { return A.value == B.value; }
	friend bool operator!=(Handle A, Handle B) { return A.value != B.value; }

	static void Register(Registry &);
};

StringStream &operator<<(StringStream &, Handle);

KAI_END

#endif // KAI_HANDLE_H

//EOF
