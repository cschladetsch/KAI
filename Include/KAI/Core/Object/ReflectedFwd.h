#pragma once

#include <KAI/Core/ReflectedBase.h>
#include "KAI/Core/Pointer.h"

KAI_BEGIN

/// A Reflected type has a Self pointer. This is semantically same as C++'s 'this' pointer,
/// but using the Object Model of the system
class Reflected: public ReflectedBase
{
public:
	Registry &Reg() const;

	template <class T>
	Pointer<T> New();

	template <class T>
	Pointer<T> New(const T& val);
};

KAI_END
