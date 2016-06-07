#include <KAI/Language/Common/ParserCommon.h>
#include <Tau/TauParser.h>

using namespace std;

TAU_BEGIN

void TauParser::Process(std::shared_ptr<Lexer> lex, Structure st)
{
	current = 0;
	indent = 0;
	lexer = lex;

	if (lexer->Failed)
	{
		Failed = true;
		Error = lexer->Error;
		return;
	}

	// strip whitespace and comments
	for (auto tok : lexer->GetTokens())
	{
		switch (tok.type)
		{
		case TokenEnum::Whitespace:
		case TokenEnum::Comment:
		case TokenEnum::NewLine:
			continue;
		}

		tokens.push_back(tok);
	}

	root = NewNode(AstEnum::Module);

	KAI_UNUSED_1(st);
	Run(Structure::Namespace);
}

void TauParser::Run(Structure st)
{
	KAI_UNUSED_1(st);
	if (tokens.empty())
		return;

	while (Current().type == TokenType::Namespace)
	{
		Consume();
		Namespace(root);
		if (Failed)
			return;
	}

	// TODO: check for invalid trailing text outside of a namespace
}

void TauParser::Namespace(AstNodePtr root)
{
	auto ns = make_shared<AstNode>(TauAstEnumType::Namespace, Consume());
	Expect(TokenEnum::OpenBrace);

	while (Current().type == TokenEnum::Class)
	{
		Consume();
		Class(ns);
		if (Failed)
			return;
	}

	Expect(TokenEnum::CloseBrace);
	root->Add(ns);
}

void TauParser::Class(AstNodePtr root)
{
	auto cl = make_shared<AstNode>(TauAstEnumType::Class, Consume());
	Expect(TokenEnum::OpenBrace);

	while (Has() && Current().type != TokenEnum::CloseBrace)
	{
		// Expect a series of methods and properties.
		// Either way, start with a type name and identifier.
		auto ty = Expect(TokenEnum::Ident)->GetToken();
		auto id = Expect(TokenEnum::Ident)->GetToken();

		if (Current().type == TokenType::OpenParan)
		{
			Consume();
			Method(cl, ty, id);
		}
		else
		{
			Field(cl, ty, id);
		}

		if (Failed)
			return;
	}

	Expect(TokenEnum::CloseBrace);

	root->Add(cl);
}

void TauParser::Method(AstNodePtr cl, TokenNode const &ty, TokenNode const &id)
{
	auto method = NewNode(AstEnum::Method, id);
	method->Add(ty);

	auto args = make_shared<AstNode>();
	method->Add(args);

	while (true)
	{
		AddArg(args);
		if (Current().type != TokenType::Comma)
			break;

		Consume();
	}

	Expect(TokenType::CloseParan);
	Expect(TokenType::Semi);

	method->Add(make_shared<AstNode>());

	cl->Add(method);
}

void TauParser::AddArg(AstNodePtr parent)
{
	auto ty = Consume();
	auto name = Consume();
	auto arg = make_shared<AstNode>();

	arg->Add(ty);
	arg->Add(name);

	parent->Add(arg);
}

void TauParser::Field(AstNodePtr cl, TokenNode const &ty, TokenNode const &id)
{
	auto field = NewNode(AstEnum::Property);
	field->Add(ty);
	field->Add(id);

	Expect(TokenType::Semi);

	cl->Add(field);
}

TAU_END

