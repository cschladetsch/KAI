
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TESTS_PROPERTIES_H
#	define KAI_TESTS_PROPERTIES_H

KAI_TESTS_BEGIN

struct TestProperties : Test::Suite<TestProperties>
{
	TestProperties()
	{
		Builder(this, "TestProperties")
			("TestConstruction", &TestProperties::TestConstruction)
			("TestInheretance", &TestProperties::TestInheretance)
			("TestBuiltins", &TestProperties::TestBuiltins)
			;
	}
	void Pre();
	void Post();

	void TestConstruction();
	void TestInheretance();
	void TestBuiltins();

protected:
	void PrepareRegistry(Registry &);
};

KAI_TESTS_END

#endif // KAI_TESTS_PROPERTIES_H

//EOF
