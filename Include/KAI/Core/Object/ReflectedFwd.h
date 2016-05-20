#pragma once

#include <KAI/Core/FwdDeclarations.h>

KAI_BEGIN

struct StorageBase;
class Registry;
template <class T> struct Pointer;
struct StorageBase;

class Reflected
{
public:
	StorageBase *Self;
	//TODO Registry *Registry;

	virtual ~Reflected() { }

	virtual void Create() { }				// called after object constructed, but before first use
	virtual bool Destroy() { return true; }	// called when object moved to deathrow, but before deleted
	virtual void Delete() { }				// called immediately before resources are released

	Registry &Reg() const;

	template <class T>
	Pointer<T> New();

	template <class T>
	Pointer<T> New(const T& val);
};

KAI_END
