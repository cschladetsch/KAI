
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TESTS_DOT_GRAPH_H
#	define KAI_TESTS_DOT_GRAPH_H

KAI_TESTS_BEGIN

struct TestDotGraph : Test::Suite<TestDotGraph>
{
	TestDotGraph()
	{
		Builder(this, "TestDotGraph")
			("TestConstruction", &TestDotGraph::Test)
			;
	}
	void Pre();
	void Post();

	void Test();
};

KAI_TESTS_END

#endif // KAI_TESTS_NAME_H

//EOF
