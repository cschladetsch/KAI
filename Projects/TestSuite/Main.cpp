#include "TestBase.h"

#include <string>

using namespace std;

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	while (!getchar());
	return result;
}