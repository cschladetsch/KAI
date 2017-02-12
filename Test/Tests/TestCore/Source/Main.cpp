#include "Test/Common.h"

using namespace std;
using namespace kai;

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	while (!getchar());
	KAI_TRACE_1(result);
	return result;
}
