#include "TestCommon.h"

extern std::list<kai::Language> TestLanguages;

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    // add language scripts to add with 1,2, ...
    for (int n = argc - 1; n > 0; --n)
    {
        TestLanguages.push_back(kai::Language(atoi(argv[n])));
    }

    return RUN_ALL_TESTS();
}
