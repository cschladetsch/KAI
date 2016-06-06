#pragma once

#include "GenerateProcess.h"

TAU_BEGIN

namespace Generate
{
	struct Proxy : GenerateProcess
	{
		virtual bool Generate(TauParser const &p, const char *fname) override;

	protected:
		virtual bool Namespace(TauParser::AstNode const &ns) override ;
		virtual bool Class(TauParser::AstNode const &cl) override ;
		virtual bool Property(TauParser::AstNode const &prop) override ;
		virtual bool Method(TauParser::AstNode const &method) override ;

		std::string ArgType(std::string &&text) const;
		std::string ReturnType(std::string &&text) const;
	};
}

TAU_END



