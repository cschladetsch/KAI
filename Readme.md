# KAI

KAI provides a network distributed **Object Model** for C++ with full reflection, persistence, and incremental garbage collection.

You can create and connect KAI nodes on different machines, connect to them, swap and monitor workloads, and remote manage all nodes in the system.

KAI comes with distributed garbage collection. It is incremental; there are no spikes in cost for the GC over time. It is smooth, and allows for a constant update loop times even with tens of thousands of objects, and with hundreds of objects being created each frame on a gaming console from 2012.

The library also offers two scripting languages: ***Pi***, which is based on RPN notation, and ***Rho***, which uses infix notation. Rho is translated to Pi.

There is also an Interface Definition Language (IDL) called ***Tau***, which is used to generate code for proxies and agents in the system.

## Getting Started

I will refer to the folder that you installed KAI into as **${KAI_HOME}**. For instance, for me on ***OSX*** and ***Ubuntu*** KAI_HOME is *~/Local/Kai*. *Local* because I made it and *Kai* because that's it's name.

I've used CMake so it should be relatively painless to make any changes to ports that I missed after updating for other ports.

### Building

See **${KAI_HOME}**/Install.md. If after reading that you still get a failed build, contact [me](matilto:christian.schladetsch@gmail.com) with the build log and platform details.

## Networking

The entire motivation for KAI was to allow for efficient, low-latency and correct networking of object state and command execution (which results in state changes!) across a group of Objects in a Registry, a group of Nodes in a Domain, and across a group of Domains in a System.

* Registry -> Object (local)
* Domain -> NetObject (shared)
* System -> Domain (shared)

A Registry is a local set of unique well-known objects. A Domain is a set of unique well-known objects within a set of network Peers. A System is a set of well-known Domains - The top-level System network does not generally deal with specific NetObjects, although it can.

Rather, a network **System** is a collection of distriuted **Domains**, each such unique Domain with a unique shared **Registry**. 

In othe words, a Registry can exist locally within a single Console applciation on a single machine. A collection of Registries is a Domain, and a collection of Domains is the overall system.

It has taken many years to build all this all up from the ground, starting with scripting languages and parsers, distributed garbage collection systems, and various fuzzy synchronisation models.

These are all coming along, slowly over the years. This project was started in 2008.

The end goal is to have a server-less, truely peer-to-peer system that yet maintains state coherency and is scalable to the tens of thousands of connected nodes. That doesn't come cheaply and work continues.

One of the first things I recognised is that it's cheap and easy to send *code* rather than send *state*. So, I started with a very brief language I called *Pi*.

Pi is _very_ fast for a computer to parse and execute, and has other benefits. However it's hard to read for a human so I created *Rho* which compiles to *Pi*.

Then I needed to generate Proxy code for Agents in the network, so why not leverage all the existing work I did on Pi and Rho for that too?

So then I refactored all the language systems to use one generic language system that can be used to generate a Lexer, Parser, AST Walker and Translator for any language you want. Overkill, but fun and practically useful. Fixing one bug fixes many languages.

But it's all for a simple and single purpose: to allow for natural and fluid networking without a client/server model. I make progress over time, and so it continues. Here's a dated overview of what I've got so far:

## Executor

A general-purpose stack-based virtual machine. Think of it like a JVM or Dalvik. The question then begs: why not use either of those. Well, I wanted two stacks (one for data, one for context). These two stacks (data and context) provide some abilities that are not avaialable on any other non-Forth based system.

## PI

	{ 1 + } 'add # 2 add & print &

This creates a function that is named 'add'. Then invokes it will an argument of 2 and prints the result.

The syntax is obtuse because it is reverse-polish notation: the arguments are introduced then an operator is aplied.

In this case, the first 'argument' is the function **{ 1 + }** which simply adds 1 to what ever is on the stack.

The next sub-sequence **'add #** stores that function to a name called "add" in the current scope.

Then we push 2 onto the stack, then the add function by name, then use the '&' operator to execute what is on the stack (the 'add' function').

Then we push the **print** function onto the stack (which is built-in), then execute that too.

The result is a stack with the integer value 3 on at the top.

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

Continuations are important because they allow you to yield the current command sequence to another without having to actually use threads.

Both Pi and Rho support continuations natively.

## Conclusion

This library will be useful to those that want to expose C++ types and instances to the runtime, and across the network.

It allows you to script C++ in a very simple way. Adding a new 'builtin type' to the system requires no macros, but just defining the type-traits for your class. No modifications to any class is required. After that you can script with any type and instances as you wish.

This means you can expose and script other classes as well, including those in a library that you do not have the source code to.

To be clear: you do not have to change the source code of a class in order to access and use it at runtime with KAI. Further, these exposed classes are directly accessable via the Network.

A REPL console application is also supplied, as well as a Window application based on FLTK.

KAI and all of its libraries and applications are completely cross-platform and supports Linux, Windows, and OSX.

Currently the networking is supplied by RakNet, but no RakNet symbols are exposed in the KAI API. As such, any networking system could be employed later without changing any of the publically visible KAI framework.

## Examples

It's hard to give examples of an engine without applications. This is an 'engine' that attempts to make C++ more dynamic and scriptable. It is also the basis for a new network model that uses *Proxies* and *Agents* to make networking as simple as async method calls.

I've since learned that this isn't uncommon anymore. Well, it was when I started it in 2009.

The best example is the REPL Window, provided in ${KAI_HOME}/Bin/Window. From there, you can see that you can change and interact with C++ objects on the fly: create them, change them, delete them. Both locally and remotely.
