Tool to Generate Network Entities
==

Networking requires **Agents** and **Proxies**. These are derived from the same
abstract interface, but have different concrete interfaces.

For instance, a Proxy will return a Future.
Agents will return direct values.

Intermediate Language
--
Some way to describe 'both sides' of the Agent/Proxy dichotomy
us required. Unfortunately this probably won't be in C++, but
use of LLVM may help here.

Code Generation
---
Abstract classes for Agents will need to be generated.

Concrete Proxy classes will need to be generated.
