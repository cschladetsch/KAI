#pragma once

#include "KAI/Core/Pointer.h"
#include "KAI/Core/StorageBase.h"
#include "KAI/Core/Registry.h"
#include "KAI/Core/Pointer.h"
#include "KAI/Core/ReflectedFwd.h"

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

