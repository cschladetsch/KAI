# TAU Source

Broken into two sections: *Language* and *Generator*

## TAU Language

This is a lexer and parser. It generates an AST that is used by the Generator

## TAU Generator Library

Uses a TAU AST to generate network  Proxy and Agent C++ class declarations and definitions. This is used by the *EntityGenerate* command-line utility to produce Proxy and Agent code from a .tau interface definition language (TAU IDL) file.

