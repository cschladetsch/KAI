# Test Systems

These are test systems based on GoogleTest of the core systems and languages created by KAI:

* Core
* Language
* Network

These share a `Common` test library.

Tests are currently built [here](Bin\Tests\Debug).

## Folders

* **Common**. Common to all tests
* **Include**. Common to all tests
* **Language**. Tests for each language.
* **Network**. Network tests.
* **Source**. General KAI Tests,

## Selecting Tests

To focus only on some tests, make a debug configuration that uses:
```bash
./TestSuite --gtest_filter=TestFoo*
```

Where *TestFoo* is the name of your test suite.
