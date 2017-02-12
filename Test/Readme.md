# Test Module

These are test systems of KAI.

There are a number of test suites. Each has their own **Readme.md**

Tests are currently built to ${KAI_HOME}/Bin, but this should change to ${KAI_HOME}/TestBin.

There should be a way to chain all the tests together to run as one unit. The best on offer at the moment is a script at the root of the installation called **RunTests** which runs all programs in **Bin/** that start with the prefix ***Test***.

There is a CMake macro to add a new test called **create_test**. 

It is defined in the top-leve _CMakeLists.txt_ file.

The way the testing system works is that the top-level CMake file **include**s the file Tests/CMakeLists.txt.

This in turn does something like:

	set(tests TestCore TestNumerics TestString TestStringStream
		TestContainers TestRegistry TestExecutor TestPi TestConsole)

	include_directories(${KAI_HOME}/Test/Include)

	foreach (test ${tests})
		create_test(${test})
	endforeach()

Which ends up buiding all the tests defined in the **tests** CMake variable.

## Auto-running tests
It would be nice to auto-run all the tests after each build. Haven't figured that out yet.

It would also be nice to have GoogleTest combine separate test suites into one, so we could pass it a list of Test executables to run and it runs them in sequence and gives a summary of **all** the tests at the end. Haven't figured this out either.


