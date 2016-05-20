#pragma once

#include <KAI/Core/Config/Base.h>

KAI_BEGIN

struct StorageBase;

/// Base for all fully Reflected<T> types
/// TODO: Now that Reflected is not a template, we don't need this at all
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

KAI_END
