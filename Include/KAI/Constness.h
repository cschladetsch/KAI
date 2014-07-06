
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_CONSTNESS_H
#	define KAI_CONSTNESS_H

KAI_BEGIN

/// Specify the constness of a thing
struct Constness 
{
	enum Type { None, Const, Mutable };
	Type value;
	Constness(Type T = None) : value(T) { }

	const char *ToString() const;
	friend bool operator==(Constness A, Constness B) { return A.value == B.value; }
	friend bool operator<(Constness A, Constness B) { return A.value < B.value; }
};

KAI_END

#endif // KAI_CONSTNESS_H

//EOF
