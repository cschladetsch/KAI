#!/usr/bin/env python3
PATH = "Ext/CppKaiCore/Source/Library/Executor/Source/Console.cpp"

with open(PATH, "r", encoding="utf-8", errors="replace", newline="") as f:
    c = f.read()

old = "out << rang::style::bold << symbol << rang::fg::reset;"
new = (
    'if (std::string_view(symbol) == "\xce\xbb ")\n'
    "        out << rang::fg::cyan << rang::style::bold << symbol << rang::fg::reset;\n"
    "    else\n"
    "        out << rang::style::bold << symbol << rang::fg::reset;"
)

if old in c:
    c = c.replace(old, new)
    print("Replaced.")
else:
    print("ERROR: pattern not found.")

with open(PATH, "w", encoding="utf-8", newline="") as f:
    f.write(c)
