#include <KAI/Language/Tau/Tau.h>
#include <KAI/Language/Tau/Generate/Proxy.h>
#include <fstream>

using namespace std;

TAU_BEGIN

namespace Generate
{
	bool Proxy::Generate(TauParser const &p, const char *fname)
	{
		auto const &root = p.GetRoot();
		if (root->GetType() != TauAstEnumType::Module)
			return Fail("Expected a Module");

		for (const auto &ch : root->GetChildren())
		{
			if (ch->GetType() != TauAstEnumType::Namespace)
				return Fail("Namespace expected");

			if (!Namespace(*ch))
				return false;
		}

		return (ofstream(fname) << Prepend() << ProxyPrepend() << _str.str() << endl).good();
	}

	string Proxy::ProxyPrepend() const
	{
		stringstream str;
		str << "#include <KAI/Network/ProxyDecl.h>\n\n";
		return move(str.str());
	}

	bool Proxy::Namespace(Node const &ns)
	{
		StartBlock(string("namespace ") + ns.GetToken().Text());
		for (auto const &ch : ns.GetChildren())
		{
			switch (ch->GetType())
			{
			case TauAstEnumType::Namespace:
				if (!Namespace(*ch))
					return false;
				break;

			case TauAstEnumType::Class:
				if (!Class(*ch))
					return false;
				break;

			default:
				KAI_TRACE_ERROR_1("Parser failed to fail");
				return Fail("[Internal] Unexpected %s in namespace", TauAstEnumType::ToString(ch->GetType()));
			}
		}

		EndBlock();
		return true;
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

		for (const auto &member : cl.GetChildren())
		{
			switch (member->GetType())
			{
			case TauAstEnumType::Class:
				return Class(*member);

			case TauAstEnumType::Property:
				if (!Property(*member))
					return false;
				break;

			case TauAstEnumType::Method:
				if (!Method(*member))
					return false;
				break;

			default:
				return Fail("Invalid class member: %s", TauAstEnumType::ToString(member->GetType()));
			}
		}

		EndBlock();
		return true;
	}

	bool Proxy::Property(Node const &prop)
	{
		auto type = prop.GetChild(0)->GetTokenText();
		auto name = prop.GetChild(1)->GetTokenText();
		_str << ReturnType(type);
		_str << " " << name << "()";
		StartBlock("");
		_str << "return Fetch<" << type << ">(\"" << name << "\");";
		EndBlock();
		_str << EndLine();
		return true;
	}

	bool Proxy::Method(Node const &method)
	{
		auto const &rt = method.GetChild(0)->GetTokenText();
		auto const &args = method.GetChild(1)->GetChildren();
		auto name = method.GetTokenText();
//		auto const &konst = method.GetChild(2);

		_str << ReturnType(rt) << " " << name << "(";
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
		StartBlock("");
		if (args.size() > 0)
		{
			_str << "StreamType args;" << EndLine();
			_str << "args";
			for (auto const &a : args)
			{
				_str << " << " << a->GetChild(1)->GetTokenText();
			}
			_str << ";" << EndLine();
			_str << "return Exec<" << rt << ">(\"" << name << "\", args);" << EndLine();
		}
		else
		{
			_str << "return Exec<" << rt << ">(\"" << name << "\");" << EndLine();
		}
		EndBlock();
		_str << EndLine();

		return true;
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

