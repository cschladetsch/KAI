mkdir build
pushd build
cmake -G "Visual Studio 15 2017 Win64" ..
msbuild NetworkGenerate.sln && ..\Bin\Test\Debug\KaiTest.exe
popd

