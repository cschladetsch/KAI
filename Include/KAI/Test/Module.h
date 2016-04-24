
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TEST_MODULE_H)
#	define KAI_TEST_MODULE_H

KAI_TEST_BEGIN

/// A named collecton of Test Suites
class Module : public Component, public Reflected//<Module>
{
	typedef Factory<Component> Suites;
	Suites suites;

public:
	Module() { }
	Module(String const &name) : Component(name) { }

	void Create();
	bool Destroy();

	void SetName(const char *name)
	{
		Name = name;
	}

	/// Add a new test Suite type with a given name
	template <class Suite>
	void AddSuite(const char *name)
	{
		suites.Register<Suite>(String(name));
	}

	int GetNumSuites() const { return (int)suites.GetTypes().size(); }

	void Pre();
	void Post();

	/// Run all registered test suits in the module.
	Object Run(bool verbose) /*const*/;

	void Run(IOutput *);

	// Run all tests using given output
	Object Run(Object output);//Pointer<IOutput *> output);

	// Run a specific test suite, using BasicOutput
	Pointer<Summary> RunSuite(String);

	// Run a specific method in a specific suite, using BasicOutput
	Pointer<Summary> RunMethod(String suite, String method);

	// Register the Module class with a Registry
	static void Register(Registry &, const char *);
};

KAI_TEST_END

KAI_BEGIN

KAI_TYPE_TRAITS(Test::Module, Number::TestModule, Properties::Reflected);

KAI_END

#endif // KAI_TEST_MODULE_H

//EOF
