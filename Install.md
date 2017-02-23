# Installing

You will need cmake >= 3.7 and OpenGL and a bunch of things like Raknet. I have made the CMake files as permissible as possible, as at worst you will just end up with a character-based terminal and no networking.

That is to say: if the build system can't locate a package, it will not fail. It will just not build anything that relies on that package.

Otherwise, do what I do and solve the dependancies problems one at a time as you try to build. I could try to give you deep instructions on how to add all dependancies on all platforms, but they would be outdated before you read them anyway. And they'd be wrong.

I did make it easier for you by providing .cmake files for each requirement in the top-level **${KAI_HOM}/CMake** folder that tries to find each package as required. This works cross-platform. Probably.

So, (deep breath), this should work on all platforms (assuming Cygwin on Windows):

  $ mdkir build && cd build && cmake .. && make
  $ cd ../Bin

At this point you shoud be in the binary executable folder of the library, with fun stuff in it like character-based **Console**, Gui-Based console (**Window**), and unit-tests in **Tests/KaiTests**. 

Check **${KAI_HOME}/Lib** for all static and dynamic libraries that should also have been built.

Go ahead and run the tests.

  $ Bin/Test/KaiTests

The only output should be green things. Nothing else flashy. If you see a red thing, that's bad and you should report it to me here as an Issue.

Otherwise, read *(this other document that doesn't exist)* on how to make a set of KAI nodes on one or more machines and connect them to each other.

We will call the folder that you installed KAI to as **${KAI_HOME}**.

From **${KAI_HOME}** do:

  $ mkdir build && cd build && cmake .. && make

No external libraries are required. If it doesn't build cleanly please contact [me](mailto:christian.schladetsch@gmail.com) with the build log and platform details.

## Outputs

Paths are given relative to ${KAI_HOME}:

* **/Bin** Contains all built executable binaries, such as the REPL _Console_, _Window_, and _NetworkGenerator_.
* **/Bin/Test** Contains all built test executables. If any fail, please contact [me](mailto:christian.schladetsch@gmail.com).
* **/Lib** Contains all built static and dynmaic libraries.
* **/Include** Is not a part of the make process:
  * Deterministic switching is done at compile time.
  * If you wish to build a binary that uses KAI code, **/Include** and **/Lib** should be added to your *include* and *library* search paths respectively.

## Supported platforms

Nominally, KAI supports:

* OSX
* Windows
* Ubuntu
* Linux in general

I do not always test everything on every platform before pushing to master branch.

If you have any problems, feel free to [mail me](mailto:christian.schladetsch@gmail.com)

## C++ Language requirements

For a long time I excluded using C++0x11. But, well, it just makes things easier and so now it builds with it. This is 2017 after all.

If in the rare case you need a fallback to a previous version, the main issue will be with *KAI/Include/Core/Detail/CallableBase.h*. Good luck with that. I removed hundreds of LOC with that simple file. Reproducing all that won't be trivial.
