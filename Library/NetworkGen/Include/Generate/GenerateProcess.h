#pragma once

#include <KAI/Language/Common/Process.h>
#include <Tau/TauParser.h>

// TODO: hide these under pimpl  (?)
#include <strstream>
#include <string>

TAU_BEGIN

namespace Generate
{
	struct GenerateProcess : Process
	{
		virtual bool Generate(TauParser const &p, const char *fname) = 0;

	protected:
		virtual bool Namespace(TauParser::AstNode const &ns) = 0;
		virtual bool Class(TauParser::AstNode const &cl) = 0;
		virtual bool Property(TauParser::AstNode const &prop) = 0;
		virtual bool Method(TauParser::AstNode const &method)  = 0;

	protected:
		std::string EndLine();
		std::stringstream _str;
		int _indentation = 0;
	};
}

TAU_END
