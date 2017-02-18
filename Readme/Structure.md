# Struture

Have C++ Source folders containing sub-folders for libs and apps.

There are various aspects to a **(sub)system**:

1. **Source**. The source code, usually in a *library*.
	* May depend on one other other *libraries*.
	* There can be internal or exernal.
1. **Test**. 	
	* Test code exercises the **Source**.
	* All code in all *libraries*, inclding compound *libraries*  are tested.
	* In special cases, such as **NetworkGenTest**, there can be two stages of building.
1. **Application**. 
	* Uses the **libraries**.
	* Incldes REPLE *Console*, *Window* etc.

All these aspects are enforced in **KAI**.

## Source

Lives in a distinct folder.

### Library

Usually lives in $Root/lib/libFoo.a

1. **Core**. Defines what an Object, Tree, Path, Registry are. Required by all other libraries.
	1. Defines all builtin and scalar types. Void, bool, int, float, string etc.
	1. Defines all containers: vector, list, set, map.
	1. Defines stream types:
		* StringStrream
		* BinaryStram
1. **Executor**. Vitual machine, persistent state
1. **LangCommon**. Base for all languages. Doesn't have tests because it's too abstract.
1. **Pi**. Test Pi functionalty natively.
1. **Console**. Test Repl console that uses either *Pi* or *Rho*.
1. **Rho**. Test Rho in Console.

## Test

Usually lives in $Root/Test/Bin. With Soure in Usually lives in $Root/Test/Source.

1. **Test** (tests everything)
NetworkGen)
1. **TestPi0**. Runs a set of Pi scrips via a Cosnsole.
1. **TestRho0**. Runs a set of Rho scrips via a Cosnsole.
1. **TestPi1**. Runs a set of more complicated Pi scrips via a Cosnsole.
1. **TestRho1**. Runs a set of more complicated Rho scrips via a Cosnsole.
1. **Tau**. Create Proxy and Agent strutures.
1. **TestNetworkGen** (test network gen)
1. **TestNetwork** (test communications via 1. **Node**. A console that can connect to other Ndoes.

## Appication

Usually lives in $Root/App

1. **Console**. REPL Console.
1. **Window**. FLTK Console.
1. **Debugger**. FLTK debuger.
1. **Generate**. Create *Agent* and *Proxy* source from **Tau** description.
1. **PluggedInConsole**. Console with dylib plugins.

## Details

They have different include paths, library paths, and output paths.

Want one top-level CMake file that can do anything.

But we want one source folder KAI/, and one Test folder KAI/Test.

## Folder Structure

* *buid*. **Ignored**
* *cmake-build-debig*. **Ignored**
* *CmakeIncludes*. **Ignored**
* *App*. Contains built binaries. **Ignored**
* *Bin*. 
	* Remove this folder. There's no need for it with App/ folder.
* **Include**. 
	* Should be able to be installed in /usr/local/include or equiv. 
	* For now, ok to have it in ~/KAI/Include
* **lib**. 
	* build static and dylib libs. again, should to into /user/local/lib, but for now ~/KAI/Lib is fine
* **Library**
	* This is the source code. Should be called Source
* **Platform**
	* This is also source code, and should be in **Source/Platform**
* Root
	* This is pretty abstract, and represents the users shell space. Should be moved
* System
	* Don't have one yet, but need a boostrapping system. This will also help with where to place _home_.
* Test
	* Should stay as it is as a top-level thing. It should be able to build and run alone.
	
