#!/usr/bin/env python3
"""
Replace popen/pclose usage in Console.cpp with Windows-safe stubs.
On Windows: functions that use popen return empty/error strings.
On Linux: unchanged.
"""

PATH = "Ext/CppKaiCore/Source/Library/Executor/Source/Console.cpp"

with open(PATH, "r", encoding="utf-8", errors="replace") as f:
    content = f.read()

# Block 1: ExecuteShellCommandWithColor - wrap the popen block
# The entire execution block from "FILE *pipe" to the closing } of if(pipe)
replacements = [
    # Block at ~line 469: ExecuteShellCommandWithColor
    (
        '    // Execute the command\n'
        '    FILE *pipe = popen(fullCommand.c_str(), "r");\n'
        '    if (pipe) {\n'
        '        // Use larger buffer to handle ANSI escape sequences properly\n'
        '        char buffer[4096];\n'
        '        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {\n'
        '            cout << buffer;\n'
        '        }\n'
        '        int exitCode = pclose(pipe);\n'
        '        if (exitCode != 0) {\n'
        '            cout << rang::fg::red << "Command exited with code: " << exitCode\n'
        '                 << rang::fg::reset << endl;\n'
        '        }\n'
        '    } else {\n'
        '        cout << rang::fg::red << "Failed to execute: " << command\n'
        '             << rang::fg::reset << endl;\n'
        '    }\n',

        '    // Execute the command (POSIX only)\n'
        '#ifndef _WIN32\n'
        '    FILE *pipe = popen(fullCommand.c_str(), "r");\n'
        '    if (pipe) {\n'
        '        char buffer[4096];\n'
        '        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {\n'
        '            cout << buffer;\n'
        '        }\n'
        '        int exitCode = pclose(pipe);\n'
        '        if (exitCode != 0) {\n'
        '            cout << rang::fg::red << "Command exited with code: " << exitCode\n'
        '                 << rang::fg::reset << endl;\n'
        '        }\n'
        '    } else {\n'
        '        cout << rang::fg::red << "Failed to execute: " << command\n'
        '             << rang::fg::reset << endl;\n'
        '    }\n'
        '#endif // _WIN32\n'
    ),
]

for old, new in replacements:
    if old in content:
        content = content.replace(old, new)
        print(f"Replaced block ({len(old)} chars -> {len(new)} chars)")
    else:
        print(f"WARNING: block not found (first 60 chars): {repr(old[:60])}")

# For remaining popen usages: wrap with _WIN32 guard more carefully
# Find any remaining popen lines
remaining = [i+1 for i, l in enumerate(content.splitlines()) if 'popen(' in l and '#' not in l]
if remaining:
    print(f"WARNING: popen still present at lines: {remaining}")
else:
    print("All popen calls handled.")

with open(PATH, "w", encoding="utf-8") as f:
    f.write(content)
print("Done.")
