# Test all Language Scripts

Runs the scripts in **/Source/Test/Scripts/** through the magic of magic.

This does much more work than is done in the hand-made tests.

The scripts are basically there to make it easier to rapidly expand what is tested without having to actually write C++ code to test it. The test and the testing is all done via scripting.

Furthermore, all the scripts are for the different languages are run using the same Test script. Weird and probably unnessary but true. See *TestLangScripts.cpp* in this folder.

## Invoking

The idea is that you pass the test executable information about what language scripts you want to run. Currently this is pretty crude:

  ./KaiTest 1 2 3

Where 1 2 3 means "Pi, Rho and Tau". Every other test is run anyway.

  ./KaiTest 2

Means "Run everything else and the Rho script tests". etc.



