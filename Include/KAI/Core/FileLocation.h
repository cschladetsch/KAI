#pragma once

#include <KAI/Core/Config/Base.h>
#include <KAI/Core/BuiltinTypes/String.h>

KAI_BEGIN

/// A location in a source file represented as the file name and a line number
struct FileLocation
{
	String file;
	String function;
	int line;

	FileLocation() : line(0) { }
	FileLocation(const char *F, int L, const char *G = "") : file(F), line(L), function(G) { }
	FileLocation(const char *G) : function(G) { }
	String ToString(bool strip_path = true) const;
};

KAI_END
