#pragma once

#include "GenerateProcess.h"

TAU_BEGIN

namespace Generate
{
	struct Agent : GenerateProcess
	{
		virtual bool Generate(TauParser const &p, const char *fname) override;

	private:
		bool Namespace(TauParser::AstNode const &ns);
		bool Class(TauParser::AstNode const &cl);
		bool Property(TauParser::AstNode const &prop);
		bool Method(TauParser::AstNode const &method);

		std::string ArgType(std::string &&text) const;
		std::string ReturnType(std::string &&text) const;
	};
}

TAU_END



