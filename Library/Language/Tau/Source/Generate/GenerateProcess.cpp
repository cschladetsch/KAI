#include <KAI/Language/Tau/Generate/GenerateProcess.h>
#include <fstream>

using namespace std;

KAI_BEGIN

string ReadTextFile(const char *fileName);

KAI_END

TAU_BEGIN

namespace Generate
{
	bool GenerateProcess::Generate(TauParser const &p, const char *fileName)
	{
		if (!Module(p))
			return false;

		return (ofstream(fileName) << CommonPrepend() << Prepend() << _str.str() << endl).good();
	}

	string GenerateProcess::CommonPrepend()
	{
		stringstream str;
		str << "// auto-generated on " << "Tuesday" << "; modify at own risk\n";
		return str.str();
	}

	bool GenerateProcess::Module(TauParser const &p)
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

		return true;
	}

	bool GenerateProcess::Namespace(Node const &ns)
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

	bool GenerateProcess::Class(Node const &cl)
	{
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

		return true;
	}

	bool GenerateProcess::Generate(const char *inputFile, const char *outputFile)
	{
		auto parser = Parse(inputFile);
		if (parser)
			return Generate(*parser, outputFile);
		return false;
	}

	shared_ptr<TauParser> GenerateProcess::Parse(const char *fname) const
	{
		Registry r;
		auto lex = make_shared<TauLexer>(ReadTextFile(fname).c_str(), r);
		if (!lex->Process())
		{
			Fail(lex->Error);
			return nullptr;
		}

		auto parser = make_shared<TauParser>(r);
		if (!parser->Process(lex, Structure::Module))
		{
			Fail(parser->Error);
			return nullptr;
		}

		return parser;
	}

	stringstream &GenerateProcess::StartBlock(const string &name)
	{
		_str << name << EndLine() << '{';
		_indentation++;
		_str << EndLine();
		return _str;
	}

	string GenerateProcess::EndLine() const
	{
		stringstream s;
		s << "\n" << move(string(_indentation, '\t'));
		return move(s.str());
	}

	void GenerateProcess::EndBlock()
	{
		_indentation--;
		_str << EndLine() << '}';
	}
}

TAU_END

