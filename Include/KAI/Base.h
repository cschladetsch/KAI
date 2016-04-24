#pragma once

#include "KAI/Config/Base.h"
#include <string>
#include <boost/static_assert.hpp>
#include "KAI/Boost.h"
#include "KAI/NonCopyable.h"
#include "KAI/Meta/Base.h"
#include "KAI/FwdDeclarations.h"
#include "KAI/Preprocessor.h"

#undef GetMessage
#undef SendMessage
#undef GetObject

KAI_BEGIN

template <class T>
void SetAbsoluteValue(T &value)
{
	if (value < 0)
		value = -value;
}

/// Used for tri-color garbage collection
struct ObjectColor
{
	enum Color
	{
		White, Grey, Black
	};
};

StringStream& operator<<(StringStream&, ObjectColor::Color);

/// Base for all fully Reflected<T> types
struct ReflectedBase
{
	StorageBase *Self;

	virtual ~ReflectedBase() { }
	
	virtual void Create() { }				// called after object constructed, but before first use
	virtual bool Destroy() { return true; }	// called when object moved to deathrow, but before deleted
	virtual void Delete() { }				// called immediately before resources are released
};

typedef ::std::size_t HashValue;

template <class T>
struct FwdBasePointer { };

template <class T>
Storage<T> *NewStorage(Registry &);

KAI_END

#include "KAI/Type/TypeAll.h"
