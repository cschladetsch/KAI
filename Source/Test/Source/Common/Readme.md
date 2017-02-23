# Common Test Classes

Contains code that is used by many other test suites in the system.

For example, **TestCommon** class provides a minimalistic *Registry* and *Tree* structure, whereas **TestLangCommon** provides a more expansive landscape including an *Executor* and direct access to _Data_ and _Context_ stacks.

These are perhaps poorly named: **TestCommon** and **TestLangCommon** do not actually perform any tests. Rather, they are base classes for other test suites.
