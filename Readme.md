# KAI

KAI provides a network distributed **Object Model** for C++ with full reflection, persistence, and incremental garbage collection. Currently supported platforms are:

1. **Windows 10** (VS 2017)
1. **Linux** (Ubuntu)
1. **macOS** (Sierra)
1. **Unity3d" (Experimental)

You can create and connect KAI nodes on different machines, connect to them, swap and monitor workloads, and remote manage all nodes in the system.

KAI comes with distributed garbage collection. It is incremental; there are no spikes in cost for the GC over time. It is smooth, and allows for a constant update loop times even with tens of thousands of objects, and with hundreds of objects being created each frame on the compute power of a gaming console from 2012.

The system also offers two scripting languages: ***Pi***, which is based on RPN notation, and ***Rho***, which uses infix notation. Rho is translated to Pi.

There is also an Interface Definition Language (IDL) called ***Tau***, which is used to generate code for proxies and agents in the system.

## Folder Structure

* *Bin*. Where to write executable output files.
* *CMake*. Auxillary CMake modules, primarily for finding other projects and libraries like Boost and GoogleTest.
* *Doc*. Generated documentation.
* *Ext*. External dependancies, primariues as git submodules.
* *Include*. Root of the global include path.
* *Lib*. Location for built static and dynamic library files.
* *Source*. The root for the source code of the project, 
* *Test*. Unit tests.

## Interesting Files

* CMakeLists.txt. Top-level Cmake file.
* Readme,md. This file.
* Install.md. SOme information on how to install and build the system.


## Getting Started

I will refer to the folder that you installed KAI into as **${KAI\_HOME}**. See [Install.md](Install.md) for installation instructions.

KAI has various dependancies, but can be built with many sub-sets. Like, if you don't want to use [ImGui](https://github.com/ocornut/imgui) you can stick with text-based (colored) [Console](Source/App/Console).

I've used CMake so it should be relatively painless to make any changes to ports.

If after reading that you still get a failed build, contact [me](matilto:christian.schladetsch@gmail.com) with the build log and platform details.

## Networking

The entire motivation for KAI was to allow for efficient, low-latency and correct networking of object state and command execution (which results in state changes!) across a group of Objects in a Registry, a group of Nodes in a Domain, and across a group of Domains in a System.

* Object -> Registry (local)
* NetObject -> Domain (shared)
* Domain -> System (shared across multiple Domains)

A *Registry* is a local set of unique well-known *Objects*. A *Domain* is a set of unique well-known objects within a set of network *Peers*. A *System* is a set of well-known Domains - The top-level System network does not generally deal with specific NetObjects, although it can.

Rather, a network **System** is a collection of distriuted **Domains**, each such unique Domain with a unique shared **Registry**. 

In other words, a Registry can exist locally within a single Console applciation on a single machine. A collection of Registries is a Domain, and a collection of Domains is the overall System.

It has taken many years to build all this all up from the ground, starting with scripting languages and parsers, distributed garbage collection systems, and various fuzzy synchronisation models.

These are all coming along, slowly over the years. This specific project was started in 2008, and even more informally back in 2001.

The end goal is to have a server-less, truely peer-to-peer system that yet maintains state coherency and is scalable to the tens of thousands of connected nodes. That doesn't come cheaply and work continues.

One of the first things I recognised is that it's cheap and easy to send *code* rather than send *state*. So, I started with a very brief language I called *Pi*.

Pi is _very_ fast for a computer to parse and execute, and has other benefits. However it's hard to read for a human so I created *Rho* which compiles to *Pi*.

Then I needed to generate Proxy code for Agents in the network, so why not leverage all the existing work I did on Pi and Rho for that too?

So then I refactored all the language systems to use one generic language system that can be used to generate a Lexer, Parser, AST Walker and Translator for any language you want. Overkill, but fun and practically useful. Fixing one bug fixes many languages.

But it's all for a simple and single purpose: to allow for natural and fluid networking without a client/server model. I make progress over time, and so it continues. Here's a dated overview of what I've got so far:

## Executor

A general-purpose stack-based virtual machine. Think of it like a JVM or Dalvik. The question then begs: why not use either of those. Well, I wanted two stacks (one for data, one for context). These two stacks (data and context) provide some abilities that are not avaialable on any other non-Forth based system.


## Conclusion

This library will be useful to those that want to expose C++ types and instances to the runtime, and across the network.

It allows you to script C++ in a very simple way. Adding a new 'builtin type' to the system requires no macros, but just defining the type-traits for your class. No modifications to any class is required. After that you can script with any type and instances as you wish.

This means you can expose and script other classes as well, including those in a library that you do not have the source code to.

To be clear: you do not have to change the source code of a class in order to access and use it at runtime with KAI. Further, these exposed classes are directly accessable via the Network.

A REPL console application is also supplied, as well as a Window application based on [Dear ImGui](https://github.com/ocornut/imgui).

KAI and all of its libraries and applications are completely cross-platform and supports Linux, Windows, and OSX.

Currently the networking is supplied by RakNet, but no RakNet symbols are exposed in the KAI API. As such, any networking system could be employed later without changing any of the publically visible KAI framework.

## Examples

It's hard to give examples of an engine without applications. This is an 'engine' that attempts to make C++ more dynamic and scriptable. It is also the basis for a new network model that uses *Proxies* and *Agents* to make networking as simple as async method calls.

I've since learned that this isn't uncommon anymore. Well, it was when I started it in 2009.

The best example is the REPL Window, provided in ${KAI_HOME}/Bin/Window. From there, you can see that you can change and interact with C++ objects on the fly: create them, change them, delete them. Both locally and remotely.
