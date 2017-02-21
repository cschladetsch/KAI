
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestXXX.h"

KAI_BEGIN

struct TestXXXAux
{
};

KAI_TYPE_TRAITS(TestXXXAux, 666, Type::Properties::None);

KAI_END

KAI_TESTS_BEGIN

void TestXXX::Test()
{
	KAI_TEST_NOT_IMPLEMENTED();
}

KAI_TESTS_END

#endif // KAI_UNIT_TESTS

//EOF
