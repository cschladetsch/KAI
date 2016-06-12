# Language
This library defines the thee key languages used by the system:
* Pi: A post-fix language inspired by forth.
* Rho: An in-fix language inspired by Python. Complies to Pi.
* Tau: An Interface Definitional Language (IDL) that generates
C++ code for both consumers and agents of a given network entity.

## Pi
Pi is a simple post-fix language with two stacks: A datatack and a 
context stack.

## Rho
Rho is a simple in-fix dynamically typed language.

## Tau
Tau is an Interface Definition Language (IDL). It is used to generate
both consumer (Proxy) and producer (Agent) code.

A single Network entity begins with a definition in the Tau language. 
From there, Proxies are used to access Agents.

# Summary
There are currently three different language systems used by the system:
- Pi
- Rho
- Tau

The first two are used to access C++ systems reflected at runtime. 

The last, Tau, is used as an IDL to generate Proxy and Agent code
that can be statically linked to executables that interact via a Node 
system.

