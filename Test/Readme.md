# Test Languages

These are test systems of the languages created by KAI.

Tests are currently built to `${KAI\_HOME}/Test/Bin`. 

## Folders

* **Include**. Common to all tests
* **Language**. Tests for each language.
* **Source**. General KAI Tests,

# Selecting Tests

To focus only on some tests, make a debug configuration that uses:

```bash
./TestSuite --gtest_filter=TestFoo*
```

Where *TestFoo* is the name of your test suite.
