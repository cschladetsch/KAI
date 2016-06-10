#pragma once

#include <KAI/Language/Tau/Generate/GenerateProcess.h>

TAU_BEGIN

namespace Generate
{
	struct Agent : GenerateProcess
	{
		using GenerateProcess::Node;

		Agent() { }
		Agent(const char *in, const char *out);

	protected:
		virtual bool Class(Node const &cl) override;
		virtual bool Property(Node const &prop) override;
		virtual bool Method(Node const &method) override;
		virtual string Prepend() const override;

		virtual string ArgType(string const &text) const override;
		virtual string ReturnType(string const &text) const override;

	private:
		struct Decl;
		void AddAgentBoilerplate(Decl const &agent);
	};
}

TAU_END

