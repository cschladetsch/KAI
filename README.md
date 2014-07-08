KAI
===

KAI provides an Object Model for C++ with full reflection, persistence, and incremental garbage collection.

The garbage collection is an important point - because it is incremental, there are no spikes in cost for the GC over time. It is smooth, and allows for a constant 60fps with tens of thousands of objects, with hundreds of objects being created each frame on a last-gen console.

The library also offers two scripting languages: Pi, which is based on RPN notation, and Rho, which uses infix notation.

Continuations (also known as co-routines or fibres) are natively supported in both Pi and Rho languages.

This library will be useful to those that want to expose their C++ types and instances to the runtime. It allows you to script C++ in a very simple way.

A REPL console application is also supplied.

To see what the language looks like, see https://github.com/cschladetsch/KAI/blob/master/Projects/Console/TestFun.kai.

For a quick intro, see unit tests at https://github.com/cschladetsch/KAI/tree/master/Projects/Library/Source/Tests.

