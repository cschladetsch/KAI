#include <KAI/Language/Tau/Generate/Agent.h>

TAU_BEGIN

namespace Generate
{
	Agent::Agent(const char *in, const char *out)
	{
		GenerateProcess::Generate(in, out);
	}

	string Agent::Prepend() const
	{
		return move(string("#include <KAI/Network/AgentDecl.h"));
	}

	struct Agent::Decl
	{
		string RootName;
		string AgentName;

		Decl(string const &root)
				: RootName(root)
		{
			AgentName = root + "Agent";
		}

		string ToString() const
		{
			stringstream decl;
			decl << "struct " << AgentName << ": AgentBase<" << RootName << ">";
			return move(decl.str());
		}
	};

	void Agent::AddAgentBoilerplate(Decl const &agent)
	{
		_str << agent.AgentName << "(Node &node, NetHandle handle) : ProxyBase(node, handle) { }" << EndLine();
		_str << EndLine();
	}

	bool Agent::Class(TauParser::AstNode const &cl)
	{
		auto decl = Decl(cl.GetToken().Text());

		StartBlock(decl.ToString());
		AddAgentBoilerplate(decl);

		GenerateProcess::Class(cl);

		EndBlock();
		return true;
	}

	bool Agent::Property(TauParser::AstNode const &prop)
	{
		return false;
	}

	bool Agent::Method(TauParser::AstNode const &method)
	{
		return false;
	}

	std::string Agent::ArgType(std::string const &text) const
	{
		return move(text);
	}

	std::string Agent::ReturnType(std::string const &text) const
	{
		return move(text);
	}
}

TAU_END
