#pragma once

#include <KAI/Core/Config/Base.h>
#include "Base.h"
#include "Pointer.h"

KAI_BEGIN

/// A Reflected type has a Self pointer. This is semantically same as C++'s 'this' pointer,
/// but using the Object Model of the system
class Reflected : public ReflectedBase
{
public:
	Registry &Reg() const
	{
		return *Self->GetRegistry();
	}

	template <class T>
	Pointer<T> New()
	{
		return Reg().New<T>();
	}

	template <class T>
	Pointer<T> New(const T& val)
	{
		return Reg().New(val);
	}
};

KAI_END
