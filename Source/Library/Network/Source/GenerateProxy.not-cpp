#include <KAI/Language/Tau/TauParser.h>
#include <KAI/Language/Tau/Generate/GenerateProxy.h>
#include <KAI/Network/Proxy.h>
#include <fstream>

using namespace std;

TAU_BEGIN

namespace Generate
{
	bool GenerateProxy::Generate(TauParser const &p, string &output)
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

        stringstream str;
        str << Prepend() << "\n" << _str.str() << ends;
        return !Failed;
	}

	bool GenerateProxy::Namespace(Node const &ns)
	{
		StartBlock(string("namespace ") + ns.GetToken().Text());
		for (auto const &ch : ns.GetChildren())
		{
			switch (ch->GetType())
			{
			case TauAstEnumType::Namespace:
				if (!Namespace(*ch))
					return false;

			case TauAstEnumType::Class:
				if (!Class(*ch))
					return false;

			default:
				KAI_TRACE_ERROR_1("Parser failed to fail");
				Fail("[Internal] Unexpected %s in namespace", TauAstEnumType::ToString(ch->GetType()));
				break;
			}
		}

		EndBlock();
		return true;
	}

	bool GenerateProxy::Class(Node const &cl)
	{
		StartBlock(string("struct ") + cl.GetToken().Text());

		for (const auto &member : cl.GetChildren())
		{
			switch (member->GetType())
			{
			case TauAstEnumType::Class:
				return Class(*member);

			case TauAstEnumType::Property:
				if (!Property(*member))
					return false;

			case TauAstEnumType::Method:
				if (!Method(*member))
					return false;

			default:
				return Fail("Invalid class member: %s", TauAstEnumType::ToString(member->GetType()));
			}
		}

		EndBlock();
		return true;
	}

	bool GenerateProxy::Property(Node const &prop)
	{
		_str
			<< ReturnType(prop.GetTokenText())
			<< prop.GetChild(1)->GetTokenText() << ';' << EndLine();
		return true;
	}

	bool GenerateProxy::Method(Node const &method)
	{
		auto const &rt = method.GetChild(0);
		auto const &args = method.GetChild(1);
//		auto const &konst = method.GetChild(2);

		_str << ReturnType(rt->GetTokenText()) << " " << method.GetTokenText() << "(";
		bool first = true;
		for (auto const &a : args->GetChildren())
		{
			if (!first)
				_str << ", ";

			auto &ty = a->GetChild(0);
			auto &id = a->GetChild(1);
			_str << ArgType(ty->GetTokenText()) << " " << id->GetTokenText();

			first = false;
		}

		return (_str << ");" << EndLine()).good();
	}

	string GenerateProxy::ReturnType(string const &text) const
	{
		return string("IFuture<") + text + "> ";
	}

	string GenerateProxy::ArgType(string const &text) const
	{
		return text;
	}
}

TAU_END
