# Kai ![Foo](Doc/swords.jpg)

[![Build status](https://ci.appveyor.com/api/projects/status/github/cschladetsch/kai?svg=true)](https://ci.appveyor.com/project/cschladetsch/kai)
[![CodeFactor](https://www.codefactor.io/repository/github/cschladetsch/kai/badge)](https://www.codefactor.io/repository/github/cschladetsch/kai)
[![License](https://img.shields.io/github/license/cschladetsch/flow.svg?label=License&maxAge=86400)](./LICENSE.txt)
[![Release](https://img.shields.io/github/release/cschladetsch/flow.svg?label=Release&maxAge=60)](https://github.com/cschladetsch/kai/releases/latest)

*Note that the Build Status is broken because this is a CMake-based project and I haven't bothered to get to work with appveyor yet*.

Kai provides three scripting languages - Pi, Rho and Tau. It was also the inspiration for the C# [Pyro](https://github.com/cschladetsch/Pyro) implementation, which was much easier due to .Net's reflection. 

This entire project has taken almost two decades, on and off, in my own time.

Kai a network distributed **Object Model** for C++ with full reflection, persistence, and incremental garbage collection. Currently supported platforms are:

1. **Windows 10** (VS 2017)
1. **Linux** (Ubuntu)
1. **macOS** (Sierra)
1. **Unity3d** (Experimental)

You can create and connect Kai nodes on different machines, connect to them, swap and monitor workloads, and remote manage all nodes in the system.

Kai comes with distributed garbage collection. It is incremental; there are no spikes in cost for the GC over time. It is smooth, and allows for a constant update loop times even with tens of thousands of objects, and with hundreds of objects being created each frame on the compute power of a gaming console from 2012.

The system also offers two scripting languages: ***Pi***, which is based on RPN notation, and ***Rho***, which uses infix notation. Rho is translated to Pi.

There is also an Interface Definition Language (IDL) called ***Tau***, which is used to generate code for proxies and agents in the system.

Refer to the [Language Systems](Include/KAI/Language) and [implementation](Source/Library).

## Folder Structure

* *Bin*. Where to write executable output files.
* *CMake*. Auxillary CMake modules, primarily for finding other projects and libraries like Boost and GoogleTest.
* *Doc*. Generated documentation.
* *Ext*. External dependancies, primariues as git submodules.
* *Include*. Root of the global include path.
* *Lib*. Location for built static and dynamic library files.
* *Source*. The root for the source code of the project, 
* *Test*. Unit tests.

## Interesting Files and lopcations

* The toplevel [Include folder](Include/KAI) and [Source Folder](Source).
* [Installation](Install.md). Some information on how to install and build the system.
* [Tests](Test/Readme.md). Describes the GoogleTests that indicate what doesn't currently work.
* The [top level CMake file](CMakeLists.txt).


## Getting Started

See [Install.md](Install.md) for installation instructions.

KAI has various dependancies, but can be built with many sub-sets. Like, if you don't want to use [ImGui](https://github.com/ocornut/imgui) you can stick with text-based (colored) [Console](Source/App/Console).

Feel free to contact [me](matilto:christian.schladetsch@gmail.com) with any questions about building or use of the system.

## Networking

The entire motivation for KAI was to allow for efficient, low-latency and correct networking of object state and command execution (which results in state changes!) across a group of Objects in a Registry, a group of Nodes in a Domain, and across a group of Domains in a System.

Read more about Kai [object and compuational distribution](Networking.md).

## Executor

A general-purpose stack-based virtual machine. I wanted two stacks (one for data, one for context). These two stacks (data and context) provide some abilities that are not avaialable on any other non-Forth based system.

## Conclusion

This library will be useful to those that want to expose C++ types and instances to the runtime, and across the network.

It allows you to script C++ in a very simple way. Adding a new 'builtin type' to the system requires no macros, but just defining the type-traits for your class. No modifications to any class is required. After that you can script with any type and instances as you wish.

This means you can expose and script other classes as well, including those in a library that you do not have the source code to.

To be clear: you do not have to change the source code of a class in order to access and use it at runtime with KAI. Further, these exposed classes are directly accessable via the Network.

A REPL [Console](Source/App/Console) is also supplied, as well as a [Windowed](Source/App/Window) application on all platforms 

Currently the networking is supplied by RakNet, but no RakNet symbols are exposed in the KAI API. As such, any networking system could be employed later without changing any of the publically visible KAI framework.

## Examples

Start withe the [Unit Tests](Test) then have a look at the [Applications](Source/App).

