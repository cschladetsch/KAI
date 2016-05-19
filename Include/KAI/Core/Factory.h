
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_FACTORY_H
#	define KAI_FACTORY_H

KAI_BEGIN

/// Toplevel interface common to all creator functors sharing a common
/// instance interface
template <class Base>
struct CreatorBase
{
	virtual Base *Create() const = 0;
};

/// Specialisation of a creator for a specified concrete type
template <class Base, class Derived>
struct Creator : CreatorBase<Base>
{
	Base *Create() const
	{ 
		return new Derived(); 
	}
};

template <class Base>
class Factory
{
public:
	typedef String Typename;
	typedef std::map<Typename, CreatorBase<Base> *> Types;

private:
	Types types;

public:
	~Factory()
	{
		Clear();
	}

	/// Get the collection of all registered types
	Types const &GetTypes() const
	{
		return types;
	}

	/// Delete all registered factories
	void Clear()
	{
		Types::const_iterator A = types.begin(), B = types.end();
		for (; A != B; ++A)
			delete A->second;
		types.clear();
	}

	/// Register a new type under a name given as a string
	template <class Derived>
	void Register(Typename const &name)
	{
		types[name] = new Creator<Base, Derived>();
	}

	/// Create a new instance given its type name as a string
	Base *Create(Typename const &name) const
	{
		Types::const_iterator type = types.find(name);
		if (type == types.end())
			return 0;
		if (type->second == 0)
			return 0;
		return type->second->Create();
	}
};

KAI_END

#endif // KAI_FACTORY_H

//EOF
