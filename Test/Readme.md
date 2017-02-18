# Test Module

These are test systems of KAI.

There are a number of test suites. Many have their own **Readme.md**

Tests are currently built to ${KAI_HOME}/Test/Bin. 

## Auto-running tests

It would be nice to auto-run all the tests after each build. Haven't figured that out yet.

It would also be nice to have GoogleTest combine separate test suites into one, so we could pass it a list of Test executables to run and it runs them in sequence and gives a summary of **all** the tests at the end. Haven't figured this out either.

### Update: Fix
To focus only on some tests, make a debug configuration that uses:

	./TestSuite --gtest_filter=TestFoo*

Where TestFoo is the name of your test suite.
