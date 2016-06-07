#include <Generate/GenerateProcess.h>

using namespace std;

TAU_BEGIN

namespace Generate
{
	string GenerateProcess::Prepend()
	{
		strstream str;
		str << "// auto-generated on " << "Tuesday" << "; modify at own risk\n\n" << ends;
		return str.str();
	}

	stringstream &GenerateProcess::StartBlock(const string &name)
	{
		_str << name << EndLine() << '{';
		_indentation++;
		return _str;
	}

	string GenerateProcess::EndLine() const
	{
		stringstream s;
		s << "\n" << move(string(_indentation, '\t'));
		return move(s.str());
	}

	void GenerateProcess::EndBlock()
	{
		_indentation--;
		_str << EndLine() << '}';
	}
}

TAU_END

