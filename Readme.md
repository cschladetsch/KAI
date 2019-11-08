# Kai ![Foo](Doc/swords.jpg)

[![Build status](https://ci.appveyor.com/api/projects/status/github/cschladetsch/kai?svg=true)](https://ci.appveyor.com/project/cschladetsch/kai)
[![CodeFactor](https://www.codefactor.io/repository/github/cschladetsch/kai/badge)](https://www.codefactor.io/repository/github/cschladetsch/kai)
[![License](https://img.shields.io/github/license/cschladetsch/flow.svg?label=License&maxAge=86400)](./LICENSE.txt)
[![Release](https://img.shields.io/github/release/cschladetsch/flow.svg?label=Release&maxAge=60)](https://github.com/cschladetsch/kai/releases/latest)

**Note that the Build Status is broken because this is a CMake-based project that also requires Boost and Raknet and so on, and I haven't bothered to get to work with appveyor yet**.

Kai is a network distributed **Object Model** for C++ with full runtime reflection, persistence, and incremental garbage collection. No Macros are needed to expose fields or methods to the scripting runtime.

Kai provides three scripting languages - Pi, Rho and Tau. It was also the inspiration for the C# [Pyro](https://github.com/cschladetsch/Pyro) implementation, which was much easier due to .Net's reflection. 

Pi is heavily influenced by Forth. It has two directly interactable stacks: one for data, and one for context. The data stack is used for operations (as is the context stack), but the context stack tells the machine `where to go next`. This is used to create the idea of a co-routine, which is then pushed up to Rho.

[Rho](Test/Language/TestRho/Scripts/Readme.md) is an infix language much like Python, LUA or Ruby, but with native continuations and the ability to inject Pi code as any factor in an expression.

This entire project has taken almost two decades, on and off, in my own time.

The general idea has always been to move algorithms and functions around the network, as well as data. In this was, real load-balancing can be conducted.

Currently supported platforms are:

1. **Windows 10** (VS 2017-19)
1. **Linux** (Ubuntu)
1. **macOS** (Sierra)
1. **Unity3d** (2017+)

You can create and connect Kai nodes on different machines, swap and monitor workloads, and remote manage all nodes in the system.

Kai comes with distributed tri-color garbage collection. It is incremental; there are no spikes in cost for the GC over time. It is smooth, and allows for a constant update loop times even with tens of thousands of objects, and with hundreds of objects being created each frame on the compute power of a gaming console from 2012.

The system also offers two scripting languages: ***Pi***, which is based on RPN notation, and ***Rho***, which uses infix notation. Rho is translated to Pi.

There is also an Interface Definition Language (IDL) called ***Tau***, which is used to generate code for proxies and agents in the system.

Refer to the [Language Systems](Include/KAI/Language) and [implementation](Source/Library).

## Console
The basic [Console](Source/App/Console) supports both Pi and Rho as a Repl shell. It works and colored on Windows, Linux, and macOs. The following is just a basic look at the shell.

![Image](Images/BasicConsole.png)

## Examples
Basic C++/runtime interaction. First, see [Sample use of non-POD structure](Test/Source/TestClassScripting.cpp). Note that to be used by KAI, the target struct or class has no conceptual or practical requirements. Specifically, it doesn't have to derive from anything and there are no macros used to expose fields or methods:

Part of the output is:

```
[----------] 1 test from TestClassScripting
[ RUN      ] TestClassScripting.Test
Info: mystruct.ToXmlString()='
<Object type='MyStruct' name=''> <!-- no name because structure is not in a dictionary -->
  <Property name='num'>42>/Property>
  <Property name='string'>Freddy</Property>
</Object>
'

Info: stream.ToString()='Handle=55, type=MyStruct '
Info: binary_stream='BinaryStream: size=32'
```

After building, you can run the the tests yourself in ```Bin/Test/KaiTest.exe```. (Or without the `.exe` on Linux of macOS of course).

Start withe the [Unit Tests](Test) then have a look at the [Applications](Source/App).

## Networking
The entire motivation for KAI was to allow for efficient, low-latency and correct networking of object state and command execution (which results in state changes!) across a group of Objects in a Registry, a group of Nodes in a Domain, and across a group of Domains in a NetworkSystem.

Read more about Kai [object and compuational distribution](Networking.md).

## Executor
A general-purpose stack-based virtual machine. I wanted two stacks (one for data, one for context). These two stacks (data and context) provide some abilities that are not avaialable on any other non-Forth based system - especially since you can readily swap between them. That is, you can move _context_ to the data-stack, do work, then push back onto the _context_ stack.

This provides for some rich computational control. This can also result in confusion, so there is a debugger and extensive (optional) tracing available.

## Folder Structure
* *Bin*. Where to write executable output files.
* *CMake*. Auxillary CMake modules, primarily for finding other projects and libraries like Boost and GoogleTest.
* *Doc*. Generated documentation.
* *Ext*. External dependancies, primarily as _git submodules_.
* *Include*. Root of the global include path. Add this to your compiler's include path.
* *Lib*. Location for built static and dynamic library files. Also add this this to your compilers library path.
* *Source*. The root for the source code of the project.
* *Test*. Unit tests. Output goes to *Bin/Tests*.

## Interesting Files and locations
* The Test scripts for [Pi](Test/Language/TestPi/Scripts) shows the basics of pi.
* Similarly, the test scripts for [Rho](Test/Language/TestRho/Scripts) show that it's a minamalist Python-like system. Note That Rho translates to pi, and Pi transcribes very efficiently from either text or binary.
* The toplevel [Include folder](Include/KAI) and [Source Folder](Source).
* [Installation](Install.md). Some information on how to install and build the system.
* [Tests](Test/Readme.md). Describes the GoogleTests that indicate what doesn't currently work.
* The [top level CMake file](CMakeLists.txt).

## Installing and Running
See [Install.md](Install.md) for installation instructions.

The project will build using _CMake_ via _Visual Studio 2019_. No more mucking around with various *nix shells on Windows.

Your safest first bet is to build the *Console* app. This gives you a *Pi* Repl console.

_RakNet_ is integrated but needs some work to get back to connecting to remote nodes etc., due to recent changes in Raknet. This is a top priority.

_KAI_ has various dependancies, but can be built with many sub-sets. That is, if you don't want to use [ImGui](https://github.com/ocornut/imgui) you can stick with text-based (colored) [Console](Source/App/Console).

Feel free to contact [me](matilto:christian.schladetsch@gmail.com) with any questions about building or use of the system.

## Conclusion
This library will be useful to those that want to expose C++ types and instances to the runtime, and across the network.

It allows you to script C++ in a very simple way. Adding a new 'builtin type' to the system requires no macros, but just defining the type-traits for your class. No modifications to any class is required. After that you can script with any type and instances as you wish.

This means you can expose and script other classes as well, including those in a library that you do not have the source code to.

To be clear: you do not have to change the source code of a class in order to access and use it at runtime with KAI. Further, these exposed classes are directly accessable via the Network.

A REPL [Console](Source/App/Console) is also supplied, as well as a [Windowed](Source/App/Window) application on all platforms 

Currently the networking is supplied by _RakNet_, but no RakNet symbols are directly exposed in the KAI API. As such, any networking system could be employed later without changing any of the publically visible KAI framework.

