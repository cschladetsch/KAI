#include "KAI/KAIClassBuilder.h"

KAI_BEGIN

void String::Register(Registry &R)
{
	ClassBuilder<String>(R, "String")
		.Methods
			("Size", &String::Size)
			("Empty", &String::Empty)
			("Clear", &String::Clear)
		;
}

KAI_END

