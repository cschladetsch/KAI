# ImGui

Include files for external library "Dear ImGui".

The idea the author had of just having files to add to a project wasn't great.

Different platforms need different files and packages and libraries.

Took some time to integrate this. Not sure I like it in *KAI/Imgui*. An alternative I tried was to put it in KAI/External but that was worse.

Currently, ImGui builds as an internal library and is used by **App/Window** application.

## RENAMED

The folder has been renamed to ImGui. Apparently GitHub (and/or git itself) doesn't mind changing case of files or folders.

But on case-sensitive systems (OSX, Linux), remember that this is called ImGui not Imgui. I'll fix this later.
