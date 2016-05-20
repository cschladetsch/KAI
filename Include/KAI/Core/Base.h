// TODO:
// this is one of the worst files in the project, and shows why

#pragma once

#include <functional>
#include <tuple>
#include <utility>
#include <memory>
#include <utility>
#include <boost/type_index.hpp>

#include <KAI/Core/Config/Base.h>
#include <KAI/Core/FwdDeclarations.h>

KAI_BEGIN

const char *ToLower(const char *str);

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

/// Base for all fully Reflected<T> types
class ReflectedBase
{
public:
	StorageBase *Self;
	//TODO Registry *Registry;

	virtual ~ReflectedBase() { }
	
	virtual void Create() { }				// called after object constructed, but before first use
	virtual bool Destroy() { return true; }	// called when object moved to deathrow, but before deleted
	virtual void Delete() { }				// called immediately before resources are released
};

typedef ::std::size_t HashValue;

template <class T>
struct FwdBasePointer { };

	/*
template <class T>
struct Storage<T>;

template <class T>
Storage<T> *NewStorage<T>(Registry &);
	 */

KAI_END

