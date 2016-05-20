#pragma once

#include "KAI/Core/Object/StorageBase.h"
#include "KAI/Core/Registry.h"
#include "KAI/Core/Object/ReflectedFwd.h"
#include "KAI/Core/Pointer.h"

KAI_BEGIN

template <class T>
Pointer<T> Reflected::New()
{
	return Reg().New<T>();
}

template <class T>
Pointer<T> Reflected::New(const T& val)
{
	return Reg().New(val);
}

KAI_END

