KAI
===
KAI provides an Object Model for C++ with full reflection, persistence, and incremental garbage collection.

The garbage collection is an important point - because it is incremental, there are no spikes in cost for the GC over time. It is smooth, and allows for a constant 60fps with tens of thousands of objects, with hundreds of objects being created each frame on a last-gen console.

The library also offers two scripting languages: Pi, which is based on RPN notation, and Rho, which uses infix notation.

Continuations (also known as co-routines or fibres) are natively supported in both Pi and Rho languages.

This library will be useful to those that want to expose C++ types and instances to the runtime. It allows you to script C++ in a very simple way. Adding a new 'builtin type' to the system requires no macros, but just defining the type-traits for your class. No modifications to any class is required. After that you can script with any type and instances as you wish.

This means you can expose and script other classes as well, including those in a library that you do not have the source code to. 

To be clear: you do not have to change the source code of a class in order to access and use it at runtime with KAI.

A REPL console application is also supplied.

To test, run the Console application.

Examples
--------

It's hard to give examples of an engine without applications. This is an 'engine' that attempts to make C++ more dynamic and scriptable. It is also the basis for a new network model that uses Proxies and Agents to make networking as simple as async method calls.

There are also two underlying scripting languages, that take advantage of the KAI Object Model.

To see what the language looks like, see https://github.com/cschladetsch/KAI/blob/master/Projects/Console/Scripts.

For a quick intro, see unit tests at https://github.com/cschladetsch/KAI/tree/master/Projects/Library/Source/Tests.

