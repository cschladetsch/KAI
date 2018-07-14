# Installing

This can be tricky, because KAI is cross-platform across maxOS, Linux and Windows. It also has 3rd party dependancies. Don't worry too much: if the 3rd party systems can't be found you will still get the [test suite](Source/App/Test) and [Console](Source/App/Console).

## Ubuntu

The same rules apply for other apt-get based systems.

### Build and Install Google Test

GoogleTest no longer automattically installs static libraries.

So, here's one way to setup Gtest:

```bash
sudo apt-get install -y libgtest-dev
pushd /tmp
mkdir gtbuild; cd gtbuild
cmake -DCMAKE_BUILD_TYPE=RELEASE /usr/src/gtest/
make
sudo mv libg* /usr/lib/
popd
rm -rf /tmp/gtbuild
```

### Build KAI

```bash
git clone --cursive git@github.com:cschladetsch/KAI.git
sudo apt-get install -y libboost-all-dev
mkdir build; cd build; cmake ..; make
```

## Windows

You will need `cmake` installed and `boost` installed with at least `chrono`, `system` and `filesystem` libraries. Then, from a Visual Studio 2017 command line, from the root of the KAI folder structure enter the following:

```
> git clone --recursive https://github.com/cschladetsch/KAI.git
> cd KAI
> mk-vs.bat
```

Alternatively, to open in Visual Studio:

```
> start build\NetworkGenerator.sln
```

## Folders

The resulting interesting folders:

* __KAI/Bin__. Executables and test suites.
* __KAI/Include__. Root C++ include folder for KAI systems.
* __KAI/Lib__. C++ static and shared libraries
