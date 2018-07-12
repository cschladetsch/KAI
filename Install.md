# Installing

## Ubuntu

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
git clone git@github.com:cschladetsch/KAI.git
git submodule init
git submodule update
sudo apt-get install -y libboost-all-dev
mkdir build; cd build; cmake ..; make
cd ../Bin; ls
```

## Windows

You will need `cmake` installed. Then, from a Visual Studio 2017 command line, from the root of the KAI folder structure enter the following:

```
> mkdir build
> cd build
> cmake -G "Visual Studio 15 2017 Win64" ..
> msbuild NetworkGenerator.sln
```

## Folders

The resulting interesting folders:

* __KAI/Bin__. Executables and test suites.
* __KAI/Include__. Root C++ include folder for KAI systems.
* __KAI/Lib__. C++ static and shared libraries