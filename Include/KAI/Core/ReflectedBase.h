#pragma once

#include <KAI/Core/Config/Base.h>
#include <KAI/Core/FwdDeclarations.h>
#include <KAI/Core/Object/StorageBase.h>

KAI_BEGIN

template <class T>
struct Pointer<T>;

struct StorageBase;

/// Base for all fully Reflected<T> types
template <class T>
Pointer<T> Reflected::New()
{
	return Self->GetRegistry().New<T>();
}

template <class T>
Pointer<T> Reflected::New(const T& val)
{
	return Self->GetRegistry().New<T>(val);
}

KAI_END
