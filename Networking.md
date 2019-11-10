# Networking

The main reason for the development of this system was to allow for fast and efficient distributed computation.

* Object -> Registry (local)
* NetObject -> Domain (shared)
* Domain -> System (shared across multiple Domains)

A *Registry* is a local set of unique well-known *Objects*. A *Domain* is a set of unique well-known objects within a set of network *Nodes*. 

A *System* is a set of well-known Domains - The top-level System network does not generally deal with specific NetObjects, although it can.

Rather, a network System is a collection of distriuted Domains, each such unique Domain with a unique shared Registry.

In other words, a Registry can exist locally within a single Console applciation on a single machine. A collection of Registries is a Domain, and a collection of Domains is the overall System.

It has taken many years to build all this all up from the ground, starting with scripting languages and parsers, distributed garbage collection systems, and various fuzzy synchronisation models.

These are all coming along, slowly over the years. This specific project was started in 2008, and even more informally back in 2001.

The end goal is to have a server-less, truely peer-to-peer system that yet maintains state coherency and is scalable to the tens of thousands of connected nodes. That doesn't come cheaply and work continues.

One of the first things I recognised is that it's cheap and easy to send code rather than send state. So, I started with a very brief language I called Pi.

Pi is very fast for a computer to parse and execute, and has other benefits. However it's hard to read for a human so I created Rho which compiles to Pi.

Then I needed to generate Proxy code for Agents in the network, so why not leverage all the existing work I did on Pi and Rho for that too?

So then I refactored all the language systems to use one generic language system that can be used to generate a Lexer, Parser, AST Walker and Translator for any language you want. Overkill, but fun and practically useful. Fixing one bug fixes many languages.

But it's all for a simple and single purpose: to allow for natural and fluid networking without a client/server model. I make progress over time, and so it continues. Here's a dated overview of what I've got so far:

## Remote Object Creation
Create a remote object, and set it's value:

```
'remote = new Peer("192.168.0.13", 6666)	# connect to remote machine
'proxy = remote.NewProxy(Vector3)			# make a vector3 over there
proxy.Set(new Vector3(1,2,3))					# set it's (remote) value
```

## Remote Object Location



## Remote Object Procedure calls

## Remote Object Properties

## Remote Object Events

## Remote Object Reactive Programming Model


