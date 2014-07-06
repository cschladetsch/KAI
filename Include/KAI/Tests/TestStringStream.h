

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TESTS_STRING_STREAM_H
#	define KAI_TESTS_STRING_STREAM_H

KAI_TESTS_BEGIN

struct TestStringStream : Test::Suite<TestStringStream>
{
	TestStringStream()
	{
		Builder(this, "TestStringStream")
			("TestConstruction", &TestStringStream::TestConstruction)
			("TestStrings", &TestStringStream::TestStrings)
			;
	}
	void TestConstruction();
	void TestStrings();
};

KAI_TESTS_END

#endif // KAI_TESTS_STRING_STREAM_H

//EOF
