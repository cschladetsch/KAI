#!/usr/bin/env python3
"""Clean fix for WritePrompt - add cyan color for lambda prompt."""
PATH = "Ext/CppKaiCore/Source/Library/Executor/Source/Console.cpp"

with open(PATH, "r", encoding="utf-8", errors="replace", newline="") as f:
    c = f.read()

# Replace the entire output section of WritePrompt
# Find the function and replace just the output line
old = '    out << rang::style::bold << symbol << rang::fg::reset;\n    out.flush();'

new = (
    '    if (lang == Language::Pi || lang == Language::Rho)\n'
    '        out << rang::style::bold << symbol << rang::fg::reset;\n'
    '    else\n'
    '        out << rang::fg::cyan << rang::style::bold << symbol << rang::fg::reset;\n'
    '    out.flush();'
)

# But lang is not in scope at the output line - restructure the whole function
old_func = '''\
void Console::WritePrompt(ostream &out) const {
    // '$' in shell mode, otherwise the language symbol.
    const char *symbol;
    if (shellMode) {
        symbol = "$ ";
    } else {
        auto lang = static_cast<Language>(compiler->GetLanguage());
        symbol = (lang == Language::Rho) ? "\u03c1 " :
                 (lang == Language::Pi)  ? "\u03c0 " : "\u03bb ";
    }

    out << rang::style::bold << symbol << rang::fg::reset;
    out.flush();  // Ensure prompt is displayed immediately
}'''

new_func = '''\
void Console::WritePrompt(ostream &out) const {
    // '$' in shell mode, otherwise the language symbol.
    const char *symbol;
    bool isLambda = false;
    if (shellMode) {
        symbol = "$ ";
    } else {
        auto lang = static_cast<Language>(compiler->GetLanguage());
        if (lang == Language::Rho)      symbol = "\u03c1 ";
        else if (lang == Language::Pi)  symbol = "\u03c0 ";
        else { symbol = "\u03bb "; isLambda = true; }
    }

    if (isLambda)
        out << rang::fg::cyan << rang::style::bold << symbol << rang::fg::reset;
    else
        out << rang::style::bold << symbol << rang::fg::reset;
    out.flush();  // Ensure prompt is displayed immediately
}'''

if old_func in c:
    c = c.replace(old_func, new_func)
    print("Replaced WritePrompt cleanly.")
else:
    print("ERROR: exact function not found.")
    idx = c.find("void Console::WritePrompt")
    print(repr(c[idx:idx+500]))

with open(PATH, "w", encoding="utf-8", newline="") as f:
    f.write(c)
