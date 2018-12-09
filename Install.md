# Installing

This can be tricky, because KAI is cross-platform across maxOS, Linux and Windows. It also has 3rd party dependancies. Don't worry too much: if the 3rd party systems can't be found you will still get the [test suite](Source/App/Test) and [Console](Source/App/Console).

## Ubuntu

```
# ./build-linux
```

## Windows

You will need `cmake` installed and `boost` installed with at least `chrono`, `system` and `filesystem` libraries. Then, from a Visual Studio 2017 command line, from the root of the KAI folder structure enter the following:

```
> .\build-vs2017.bat
```

To open in _Visual Studio 2017_ (after running the above build command):

```
> start build\NetworkGenerator.sln
```

## Folders

The resulting interesting folders:

* __KAI/Bin__. Executables and test suites.
* __KAI/Include__. Root C++ include folder for KAI systems.
* __KAI/Lib__. C++ static and shared libraries
