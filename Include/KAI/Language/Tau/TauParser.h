#pragma once

#include <KAI/Language/Common/ParserCommon.h>
#include <KAI/Language/Tau/TauLexer.h>
#include <KAI/Language/Tau/TauAstNode.h>

TAU_BEGIN

// parser specific to the IDL Tau language
class TauParser : public ParserCommon<TauLexer, TauAstEnumType>
{
public:
	typedef ParserCommon<TauLexer, TauAstEnumType> Parent;
	using typename Parent::TokenEnum;
	using typename Parent::TokenNode;
	using typename Parent::Lexer;
	using typename Parent::AstNode;
	using typename Parent::AstNodePtr;

	typedef TauAstEnumType::Enum AstType;
	typedef TauAstEnumType NodeType;
	typedef TauTokenEnumType TokenType;

	TauParser(Registry &r) : Parent(r) { }

	virtual bool Process(std::shared_ptr<Lexer> lex, Structure st) override;

protected:
	bool Run(Structure);

	void Namespace(AstNodePtr rent);
	void Class(AstNodePtr rent);
	void Method(AstNodePtr klass, TokenNode const &returnType, TokenNode const &name);
	void Field(AstNodePtr klass, TokenNode const &ty, TokenNode const &id);
	//void Event(AstNodePtr klass, TokenNode const &ty, TokenNode const &id);

	void AddArg(AstNodePtr parent);
	void OptionalSemi();
};

TAU_END
