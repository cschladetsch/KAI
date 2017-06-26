# Installing

## Build and Install Google Test

GoogleTest no longer automattically installs static libraries. 

So, here's one way to setup Gtest:

`
sudo apt-get install -y libgtest-dev 
pushd /tmp
mkdir gtbuild
cd gtbuild
cmake -DCMAKE_BUILD_TYPE=RELEASE /usr/src/gtest/
make
sudo mv libg* /usr/lib/
popd
rm -rf /tmp/gtbuild
`

## Build KAI

`
git clone git@github.com:cschladetsch/KAI.git
git submodule init
sudo apt-get install -y libboost-all-dev
mkdir build; cd build; cmake ..; make
cd ../Bin; ls
`

## Folders

The resulting interesting folders:

* KAI/Bin. Executables and test suites.
* KAI/Include. Root C++ include folder for KAI systems.
* KAI/Lib. C++ static and shared libraries

