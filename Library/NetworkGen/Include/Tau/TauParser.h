
#pragma once

#include <KAI/Language/Common/Structure.h>
#include <KAI/Language/Common/ParserCommon.h>

#include "Tau/TauToken.h"
#include "Tau/TauAstNode.h"
#include "Tau/TauLexer.h"

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

	virtual void Process(std::shared_ptr<Lexer> lex, Structure st) override;

//	void Walk(AstNodePtr node, std::function<bool(AstNodePtr)> fun) const;

protected:
	void Run(Structure);

	void Namespace(AstNodePtr rent);
	void Class(AstNodePtr rent);
	void Method(AstNodePtr klass, TokenNode const &returnType, TokenNode const &id);
	void Field(AstNodePtr klass, TokenNode const &ty, TokenNode const &id);
	void AddArg(AstNodePtr parent);
	void OptionalSemi();
};

TAU_END
