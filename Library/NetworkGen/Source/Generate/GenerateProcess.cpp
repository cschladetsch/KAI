#include <Generate/GenerateProcess.h>
#include <KAI/Language/Common/ParserCommon.h>

using namespace std;

TAU_BEGIN

namespace Generate
{
	string GenerateProcess::EndLine()
	{
		_str << "\n";
		return move(string(_indentation, '\t'));
	}

}

TAU_END
