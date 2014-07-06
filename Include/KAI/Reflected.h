
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_REFLECTED_H
	#define KAI_REFLECTED_H

KAI_BEGIN

/// A Reflected type has a Self pointer. This is semantically same as C++'s 'this' pointer,
/// but using the Object Model of the system
template <class T>
struct Reflected : ReflectedBase
{
	T *This;

	template <class U>
	Pointer<U> New() const
	{
		return Self->GetRegistry()->New<U>();
	}

	template <class U>
	Pointer<U> NewRetained() const
	{
		return Self->GetRegistry()->NewRetained<U>();
	}

	template <class U>
	Pointer<U> New(const U &X) const
	{
		return Self->GetRegistry()->New<U>(X);
	}

	Object NewFromTypeNumber(Type::Number type_number)
	{
		return Self->NewFromTypeNumber(type_number);
	}

	Object NewFromClassName(String const &type_name) const
	{
		return Self->NewFromClassName(type_name.c_str());
	}

	template <class U>
	Storage<U> *NewStorage() const
	{
		return Self->GetRegistry()->NewStorage<U>();
	}
};

KAI_END

#endif // KAI_REFLECTED_H

//EOF
