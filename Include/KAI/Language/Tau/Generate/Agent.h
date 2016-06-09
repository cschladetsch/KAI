#pragma once

#include <KAI/Language/Tau/Generate/GenerateProcess.h>

TAU_BEGIN

namespace Generate
{
	struct Agent : GenerateProcess
	{
		using GenerateProcess::Node;

		Agent(const char *in, const char *out);

		virtual bool Generate(TauParser const &p, const char *fileName) override;

	protected:
		virtual bool Namespace(Node const &ns) override;
		virtual bool Class(Node const &cl) override;
		virtual bool Property(Node const &prop) override;
		virtual bool Method(Node const &method) override;

		virtual std::string ArgType(std::string const &text) const override;
		virtual std::string ReturnType(std::string const &text) const override;
	};
}

TAU_END

