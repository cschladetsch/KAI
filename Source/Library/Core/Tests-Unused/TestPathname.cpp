
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestPathname.h"

KAI_TESTS_BEGIN

void TestPathname::TestConstruction()
{
	Pathname P("/");
	KAI_TEST_TRUE(P.Absolute);
	KAI_TEST_TRUE(P.GetElements().empty());

	P.FromString("foo");
	KAI_TEST_FALSE(P.Absolute);
	KAI_TEST_EQUIV(P.GetElements().size(), std::size_t(1));
	KAI_TEST_EQUIV(P.GetElements().front().type, Pathname::Element::Name);
	KAI_TEST_EQUIV(P.GetElements().front().name, "foo");
	KAI_TEST_EQUIV(P.ToString(), "foo");

	P.FromString("/foo/bar/");
	KAI_TEST_TRUE(P.Absolute);
	KAI_TEST_EQUIV(P.GetElements().size(), std::size_t(3));
	
	String S = P.ToString();
	KAI_TEST_EQUIV(S, "/foo/bar");
}

KAI_TESTS_END

#endif

//EOF
