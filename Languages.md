# KAI Languages

The KAI system comes with a generic language-consruction system that resides [here](KAI/Include/KAI/Language/Common).

## PI

[Pi](KAI/Include/KAI/Language/Pi) us a post-fix language inspired by the amazing [HP48](https://en.wikipedia.org/wiki/HP_48_series) calculator.

Here's some sample Pi script. Note that it's not expected that you haveto write Pi code by hand:

```
{ 1 + } 'add # 2 add & print &
```

This creates a function that is named 'add'. Then invokes it will an argument of 2 and prints the result.

The syntax is obtuse because it is reverse-polish notation: the arguments are introduced then an operator is aplied.

In this case, the first 'argument' is the function **{ 1 + }** which simply adds 1 to what ever is on the stack.

The next sub-sequence **'add #** stores that function to a name called "add" in the current scope.

Then we push 2 onto the stack, then the add function by name, then use the '&' operator to execute what is on the stack (the 'add' function').

Then we push the **print** function onto the stack (which is built-in), then execute that too.

The result is an empty stack with the console output of "3".

Im summary: It creates a coroutine __{ 1 + }__ which adds 1 to whatever is on the stack. It then stores this to an object called add with __'add #__. It then pushes 2 and the coro onto the stack, resumes the coro with __&__, pushes **print** onto the stack and resumes that. The result is **3**.

## Rho

[Rho](KAI/Include/KAI/Language/Pi) is an *in-fix* language that translates to Pi.

Sample Rho looks a lot like Python:

```
fun add(a)
    return a+1
print(add(2))
```

These two examples written in *Pi* and *Rho* functionally equivalent, and both output the number **3**. 


The **Rho** example is more familiar. Define a function that takes one argument and return it plus 1. Then print the result of calling that function with the argument 2.

In both cases, the data stack will be left empty and the console will have output of '3'. 

Rho gets translated to Pi code on the fly, so when you 'compile' the Rho code above you will get something very similiar to the pi code above that.

## Tau

**Tau** is the language used to describe objects that are visible across a network. It all works, but I don't have documentation for it yet. Basically, Tau takes a foo.tau file, and generates foo.agent.cpp/h and foo.proxy.cpp/h. If you want to host a Foo, you implement what's required in the foo.agent.cpp file, else if you want to use a Foo from somewhere else, you just use kai::Proxy<Foo>.

### Continuations

Also known as co-routines or fibres, are natively supported in both Pi and Rho languages.

Continuations are important because they allow you to yield the current command sequence to another without having to actually use threads.

Since I started making this system in 2000, the idea of Continuations has become common place and alsp called different things. Basically, a continuation is a function that can be interrupted at a certain point, then later resumed from that point.

What makes Pi and Rho interesting is that they use **two stacks** - one for context and one for data.

I am not sure of any other language system othe than [Forth](https://en.wikipedia.org/wiki/Forth_\(programming_language\)) that does this,
