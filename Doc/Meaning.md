# Semantics

So, why make this?

* Cross platform communication using native C/C++
* Distributed Garbage Collection
* Easy to expose C/C++ structures and values to
  * Your programs
  * Scripting (with not one but *three* languages!)
  * Your shell

## First Application

Bash++. Be a shell you can use daily across many machines. Integrate C/C++ seamlessly and type-safely.

* A cross-platform shell that runs on all your machines (OSX, Windows, Ubuntu) with no setup cost.
  * Default to native shell. All commands preceeded by backtick or whatever will be redirected to KAI.
  * This wouldn't require a change to any shells. Just a script or two.
* Changes made to start-up scripts and configs are automatically replicated
* Easy to use, fun cross-platform shell. Make work/life easier to sync outside of Git.
* Pipes etc are already built in via Channel\<**T**\> and Future\<**T**\>
* Be a 21C shell

## Second Application

Other uses include:

1. Research project.
1. ~32 ppl FPV shooters with ~10ms latency
1. ~32k ppl MMOGs with ~150ms latency
1. ???
1. Profit
