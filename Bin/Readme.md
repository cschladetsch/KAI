# Bin

This is where all the KAI applications live after they are built.

This is largely a place-holder folder, but the font(s) are used by the Window application.

I am not keen on the usual Unix approach of "installing" things into weird and wonderful directories. Sometimes /opt/local, sometimes /usr/local, or whatever. Then you need magic scripts to find things and gah.

The approach I've taken here is that KAI is entirely self-contained. You download it build it from source, and are left with three key folders at the root of your KAI installation:

* ***Lib*** Built static and dynamic libraries. Add this to your linker path if needed.
* ***Bin*** Built applications like the console REPL and the Window gui and the NeworkGenerator. Add to PATH if needed.
* ***Include*** Under this is the "real" KAI include folder. So if you installed to Foo, add -IFoo/Include so you can use *\<KAI/Core/Header.h\>* or whatever.

Perhaps I'm wrong. But for small(?) projects like this, it's best to keep things local and removable with a single rm -rf command.
