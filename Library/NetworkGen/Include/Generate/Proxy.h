#pragma once

#include <KAI/Language/Common/Process.h>
#include <Tau/TauParser.h>

// TODO: hide these under pimpl
#include <strstream>
#include <iostream>
#include <string>

TAU_BEGIN

	namespace Generate
	{
		struct Proxy : Process
		{
			bool Generate(TauParser const &p, const char *fname);

		private:
			std::stringstream _str;
			int _indentation = 0;

			std::string EndLine();

			bool Namespace(TauParser::AstNode const &ns);
			bool Class(TauParser::AstNode const &cl);
			bool Property(TauParser::AstNode const &prop);
			bool Method(TauParser::AstNode const &method);

			std::string ArgType(std::string &&text) const;
			std::string ReturnType(std::string &&text) const;
		};
	}

TAU_END



