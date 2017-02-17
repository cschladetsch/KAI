# KAI Include

The root of the KAI system.

Will beed to be re-organised so that users can

	#include "KAI/Console.h"		// just get access to REPL Console
	#include "KAI/Registry.h"		// just for object creation and persistence
	#include "KAI/Network.h"		// just for type-safe remoting etc.

and so on.

Don't really want to add a

	#include "KAI/KAI.h"

Except one exists and just includes ***KAI/Executor/Executor.h***. Which to be honest includes most of the entire system anyway.

## Console
A cross-platform REPL console for Pi or Rho languages. Then language can be switched on the fly.

## Core
The root library. Defines what an Object is, how they work, and also containers, Reegistries etc. More detail in that folder.

## Executor
This is a common virtual machine that uses two stacks: one for data and one for context. This is based on ideas from Forth and the like.

## Language
There are currently three languages in the KAI system:

1. Pi. Post-fix, forth-like.
2. Rho. In-fix, python-like.
3. Tau. An IDL that generates C++ code for C++ agents and proxies.

The languages are all located in <KAI/Lang/Lang.h> for each of Pi, Rho and Tau for _Lang_.

## Network
The common networking system. This is entirely indepedant of actual transport layers or libraries. Currently first implementation is based on RakNet, but that is not needed to build - only to link.

## Platform
Contains platform-specific code. Currently supported platforms are:

* OSX
* Ubuntu
* Windows 7-10

The CMakeFiles should deal with the details, however you may need to install boost and RakNet and provide some hints to their location.
