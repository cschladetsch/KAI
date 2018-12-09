#include <KAI/Language/Tau/Tau.h>
#include <KAI/Language/Tau/Generate/Proxy.h>

using namespace std;

TAU_BEGIN

namespace Generate
{
    Proxy::Proxy(const char *in, const char *out)
    {
        GenerateProcess::Generate(in, out);
    }

    string Proxy::Prepend() const
    {
        return move(string("#include <KAI/Network/ProxyDecl.h>\n\n"));
    }

    struct Proxy::ProxyDecl
    {
        string RootName;
        string ProxyName;

        ProxyDecl(string const &root)
            : RootName(root)
        {
            ProxyName = root + "Proxy";
        }

        string ToString() const
        {
            stringstream decl;
            decl << "struct " << ProxyName << ": ProxyBase";
            return move(decl.str());
        }
    };

    void Proxy::AddProxyBoilerplate(ProxyDecl const &proxy)
    {
        _str << "using ProxyBase::StreamType;" << EndLine();
        _str << proxy.ProxyName << "(Node &node, NetHandle handle) : ProxyBase(node, handle) { }" << EndLine();
        _str << EndLine();
    }

    bool Proxy::Class(Node const &cl)
    {
        auto decl = ProxyDecl(cl.GetToken().Text());

        StartBlock(decl.ToString());
        AddProxyBoilerplate(decl);

        GenerateProcess::Class(cl);

        EndBlock();
        return true;
    }

    bool Proxy::Property(Node const &prop)
    {
        auto type = prop.GetChild(0)->GetTokenText();
        auto name = prop.GetChild(1)->GetTokenText();
        _str << ReturnType(type);
        _str << " " << name << "()";
        StartBlock();
        _str << "return Fetch<" << type << ">(\"" << name << "\");";
        EndBlock();
        _str << EndLine();
        return true;
    }

    bool Proxy::Method(Node const &method)
    {
        auto const &returnType = method.GetChild(0)->GetTokenText();
        auto const &args = method.GetChild(1)->GetChildren();
        auto name = method.GetTokenText();

        MethodDecl(returnType, args, name);
        MethodBody(returnType, args, name);

        _str << EndLine();

        return true;
    }

    void Proxy::MethodDecl(const string &returnType, const Node::ChildrenType &args, const string &name)
    {
        _str << ReturnType(returnType) << " " << name << "(";
        bool first = true;
        for (auto const &a : args)
        {
            if (!first)
                _str << ", ";

            auto &ty = a->GetChild(0);
            auto &id = a->GetChild(1);
            _str << ArgType(ty->GetTokenText()) << " " << id->GetTokenText();

            first = false;
        }
        _str << ")";
    }

    string ReturnLead(string const &rt, string const &name)
    {
        stringstream str;
        str << "return Exec<" << rt << ">(\"" << name << "\"";
        return move(str.str());
    }

    void Proxy::MethodBody(const string &returnType, const Node::ChildrenType &args, const string &name)
    {
        StartBlock();
        auto ret = ReturnLead(returnType, name);
        if (args.size() > 0)
        {
            _str << "StreamType args;" << EndLine();
            _str << "args";
            for (auto const &a : args)
            {
                _str << " << " << a->GetChild(1)->GetTokenText();
            }
            _str << ";" << EndLine();
            _str << ret << ", args);";
        }
        else
        {
            _str << ret << ");";
        }

        _str << EndLine();
        EndBlock();
    }

    string Proxy::ReturnType(string const &text) const
    {
        return move(string("Future<") + text + ">");
    }

    string Proxy::ArgType(string const &text) const
    {
        return move(text);
    }
}

TAU_END

