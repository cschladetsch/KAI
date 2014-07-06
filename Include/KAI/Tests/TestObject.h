

#if defined(KAI_UNIT_TESTS) && !defined(KAI_TESTS_OBJECT_H)
#	define KAI_TESTS_OBJECT_H

KAI_TESTS_BEGIN

struct TestObject : Test::Suite<TestObject>
{
	TestObject()
	{
		Builder(this, "TestObject")
			("TestConstruction", &TestObject::TestConstruction)
			("TestDirtyBit", &TestObject::TestDirtyBit)
			("TestDictionary", &TestObject::TestDictionary)
			("TestAttributes", &TestObject::TestAttributes)
			("TestDictionary", &TestObject::TestDictionary)
			;
	}
	void TestConstruction();
	void TestDictionary();
	void TestDirtyBit();
	void TestAttributes();
};

KAI_TESTS_END

#endif // KAI_TEST_OBJECT_H

//EOF
