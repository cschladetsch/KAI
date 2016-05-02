#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Structure.h"
#include "KAI/Translator/Pi/PiToken.h"
#include "KAI/Translator/Pi/PiParser.h"

#include <memory>
#include <iostream>
#include <algorithm>
#include <strstream>
#include <stdarg.h>

KAI_BEGIN

bool PiParser::Program()
{
	while (!Try(TokenType::None) && !Failed)
	{
	}

	return true;
}

KAI_END
