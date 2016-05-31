# KAI

KAI is .NET for C++, without the overhead or runtime cost.

KAI provides a distributed Object Model for C++ with full reflection, persistence, and incremental garbage collection. You can create and connect KAI nodes on different machines, connect to them, swap and monitor workloads, and remote manage all nodes in the system.

The system comes with distributed garbage collection. It is incremental; there are no spikes in cost for the GC over time. It is smooth, and allows for a constant 60fps with tens of thousands of objects, with hundreds of objects being created each frame on a gaming console from 2008.

The library also offers two scripting languages: Pi, which is based on RPN notation, and Rho, which uses infix notation. Rho is translated to Pi. 

There is also an Interface Definition Language (IDL) called Tau, which is used to generate code for proxies and agents in the system. To give you and idea:

## PI

	{ 1 + } 'add # 2 add & print &

## Rho

	fun add(a)
		return a+1
	print(add(2))

These two examples are functionally equivalent, and both return **3**. 

The first is post-fix **Pi** and will not be too strange that those that have used Forth or HP48 calculators. It creates a coroutine __{ 1 + }__ which adds 1 to whatever is on the stack. It then stores this to an object called add with __'add #__. It then pushes 2 and the coro onto the stack, resumes the coro with __&__, pushes **print** onto the stack and resumes that. The result is **3**.

The **Rho** example is more familiar. Define a function that takes one argument and return it plus 1. Then print the result of calling that function with the argument 2.

In both cases, the data stack will be left with the single value 'int 3'. Rho gets translated to Pi code on the fly, so when you 'compile' the Rho code above you will get something very similiar to the pi code above that.

## Tau
**Tau** is the language used to describe objects that are visible across a network. It all works, but I don't have documentation for it yet. Basically, Tau takes a foo.tau file, and generates foo.agent.cpp/h and foo.proxy.cpp/h. If you want to host a Foo, you implement what's required in the foo.agent.cpp file, else if you want to use a Foo from somewhere else, you just use kai::Proxy<Foo>.

This is all clear as mud, I realise. But it does all work, and once I finish the libraries I will work on real documentation.

### Continuations
Also known as co-routines or fibres, are natively supported in both Pi and Rho languages.

## Conclusion
This library will be useful to those that want to expose C++ types and instances to the runtime. It allows you to script C++ in a very simple way. Adding a new 'builtin type' to the system requires no macros, but just defining the type-traits for your class. No modifications to any class is required. After that you can script with any type and instances as you wish.

This means you can expose and script other classes as well, including those in a library that you do not have the source code to. 

To be clear: you do not have to change the source code of a class in order to access and use it at runtime with KAI.

A REPL console application is also supplied, as well as a Window application based on FLTK.

KAI and all of its libraries and applications are completely cross-platform and supports Linux, Windows, and OSX.

# Installing
You will need CMake version 3.4 or greater.

From top-level do:

	# mkdir build && cd build && cmake .. && make

For the basics, no external libraries are required.

For the Window app to build, you will need [FLTK](www.fltk.org) and all of its' requirements as well.

# Examples
It's hard to give examples of an engine without applications. This is an 'engine' that attempts to make C++ more dynamic and scriptable. It is also the basis for a new network model that uses Proxies and Agents to make networking as simple as async method calls.

The best example is the REPL Window, provided. From there, you can see that you can change and interact with C++ objects on the fly: create them, change them, delete them. Both locally and remotely.

To see what the language looks like, see https://github.com/cschladetsch/KAI/blob/master/Projects/Console/Scripts.

For a quick intro, see unit tests at https://github.com/cschladetsch/KAI/tree/master/Projects/Library/Source/Tests.
