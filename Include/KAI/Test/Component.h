

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TEST_COMPONENT_H)
#	define KAI_TEST_COMPONENT_H

KAI_TEST_BEGIN

/// A component in the test system
class Component
{
protected:
	typedef nstd::list<Component *> Children;
	Children children;

public:
	Component() { }
	Component(String const &N) : Name(N) { }

	virtual void Pre() = 0;
	virtual void Post() = 0;
	virtual void Run(IOutput *) = 0;

public:
	String Name;
};

KAI_TEST_END

#endif // KAI_TEST_COMPONENT_H

//EOF
