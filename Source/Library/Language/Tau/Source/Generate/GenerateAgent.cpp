#include <KAI/Language/Tau/Generate/GenerateAgent.h>

TAU_BEGIN

namespace Generate
{
    GenerateAgent::GenerateAgent(const char *input, string &output)
    {
        GenerateProcess::Generate(input, output);
    }

    string GenerateAgent::Prepend() const
    {
        return string("#include <KAI/Network/AgentDecl.h");
    }

    bool GenerateAgent::Generate(TauParser const &p, string &output)
    {
        return false;
    }

    bool GenerateAgent::Namespace(Node const &cl)
    {
        return true;
    }

    struct GenerateAgent::Decl
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
            return decl.str();
        }
    };

    bool GenerateAgent::Class(TauParser::AstNode const &cl)
    {
        auto decl = Decl(cl.GetToken().Text());

        StartBlock(decl.ToString());
        AddAgentBoilerplate(decl);

        GenerateProcess::Class(cl);

        EndBlock();
        return true;
    }

    bool GenerateAgent::Property(TauParser::AstNode const &prop)
    {
        return false;
    }

    bool GenerateAgent::Method(TauParser::AstNode const &method)
    {
        return false;
    }

    std::string GenerateAgent::ArgType(std::string const &text) const
    {
        return text;
    }

    std::string GenerateAgent::ReturnType(std::string const &text) const
    {
        return text;
    }

    void GenerateAgent::AddAgentBoilerplate(Decl const &agent)
    {
        _str << agent.AgentName << "(Node &node, NetHandle handle) : ProxyBase(node, handle) { }" << EndLine();
        _str << EndLine();
    }
}

TAU_END

//EOF

