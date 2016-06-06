#include <Tau/TauParser.h>
#include <Generate/Proxy.h>
#include <fstream>

using namespace std;

TAU_BEGIN

	namespace Generate
	{

		bool Proxy::Generate(TauParser const &p, const char *fname)
		{
			TauParser::AstNodePtr root = p.GetRoot();
			switch (root->GetType())
			{
			case TauAstEnumType::Namespace:
			{
				if (!Namespace(*root))
					return false;
				fstream f(fname);
				const string &s = _str.str();
				return f.write(s.c_str(), s.size()).good();
			}
			}

			return Fail("Namespace expected");
		}

		string Proxy::EndLine()
		{
			_str << "\n";
			return move(string(_indentation, '\t'));
		}

		bool Proxy::Namespace(TauParser::AstNode const &ns)
		{
			_str << "namespace " << ns.GetToken().Text() << EndLine() << '{';
			_indentation++;
			for (auto const &ch : ns.GetChildren())
			{
				switch (ch->GetType())
				{
				case TauAstEnumType::Class:
					return Class(*ch);
				}
			}
			_indentation--;
			return false;
		}

		bool Proxy::Class(TauParser::AstNode const &cl)
		{
			_str << "class " << cl.GetToken().Text() << EndLine() << '{';
			_indentation++;
			for (const auto &member : cl.GetChildren())
			{
				switch (member->GetType())
				{
				case TauAstEnumType::Property:
					if (!Property(*member))
						return false;

				case TauAstEnumType::Method:
					if (!Method(*member))
						return false;

				default:
					return Fail("Unknown class member: %s", TauAstEnumType::ToString(member->GetType()));
				}
			}

			_indentation--;
			_str << '}' << EndLine();
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
			auto const &name = method.GetChild(1);
			auto const &args = method.GetChild(2);
//		auto const &konst = method.GetChild(3);

			_str << "IFuture<" << rt->GetTokenText() << "> " << name->GetTokenText() << "(";
			bool first = true;
			for (auto const &a : args->GetChildren())
			{
				if (!first)
					_str << ", ";

				auto &ty = a->GetChild(0);
				auto &id = a->GetChild(1);
				_str << ty->GetTokenText() << " " << id->GetTokenText();

				first = false;
			}

			_str << ");" << EndLine();
		}

	}

TAU_END

