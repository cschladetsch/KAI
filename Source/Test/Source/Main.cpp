#include <gtest/gtest.h>
#include <list>

#include <KAI/Core/Config/Base.h>
#include <KAI/Language/Common/Language.h>

extern std::list<kai::Language> TestLanguages;

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);

	// will use boost::program_options eventually...
	// but for now, invoke test with 
	//		KaiTest [langs]
	// where [langs] is a sequence of languages to test. 
	// See Kai/Language/Common/Language.h. Just use the enum values as ints
	//
	// For instance to test everything but languages:
	//		./KaiTest
	// Test to Pi:
	//		./KaiTest 1
	// Test Pi and Rho:
	//		./KaiTest 1 2
	for (int n = argc - 1; n > 0; --n)
	{
		TestLanguages.push_back(kai::Language(atoi(argv[n])));
	}

	return RUN_ALL_TESTS();
}
