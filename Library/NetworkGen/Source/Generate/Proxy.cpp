#include <Tau/TauParser.h>
#include <Generate/Proxy.h>
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

		fstream f(fname);
		const string &s = Prepend() + "\n" + _str.str();
		return f.write(s.c_str(), s.size()).good();
	}

	bool Proxy::Namespace(TauParser::AstNode const &ns)
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
	}

	bool Proxy::Class(TauParser::AstNode const &cl)
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
	}

	bool Proxy::Property(TauParser::AstNode const &prop)
	{
		_str
			<< "IFuture<" << prop.GetChild(0)->GetTokenText() << "> "
			<< prop.GetChild(1)->GetTokenText() << ';' << EndLine();
		return true;
	}

	bool Proxy::Method(TauParser::AstNode const &method)
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

	string Proxy::ReturnType(string &&text) const
	{
		return move(string("IFuture<") + text + "> ");
	}

	string Proxy::ArgType(string &&text) const
	{
		return move(text);
	}
}

TAU_END

