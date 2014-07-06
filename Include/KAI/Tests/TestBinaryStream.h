

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TESTS_BINARY_STREAM_H
#	define KAI_TESTS_BINARY_STREAM_H

KAI_TESTS_BEGIN

struct TestBinaryStream : Test::Suite<TestBinaryStream>
{
	TestBinaryStream()
	{
		Builder(this, "TestBinaryStream")
			("TestBuiltins", &TestBinaryStream::TestBuiltins)
			("TestObject", &TestBinaryStream::TestObject)
			("TestProperties", &TestBinaryStream::TestProperties)
			("TestList", &TestBinaryStream::TestList)
			("TestArray", &TestBinaryStream::TestArray)
			("TestMap", &TestBinaryStream::TestMap)
			("TestSet", &TestBinaryStream::TestSet)
			("TestStreams", &TestBinaryStream::TestStreams)
			;
	}
	void TestBuiltins();
	void TestObject();
	void TestProperties();
	void TestList();
	void TestArray();
	void TestMap();
	void TestSet();
	void TestStreams();
};

KAI_TESTS_END

#endif // KAI_TESTS_BINARY_STREAM_H

//EOF
