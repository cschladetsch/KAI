# Installing

You will need CMake version 3.7 or greater.

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
