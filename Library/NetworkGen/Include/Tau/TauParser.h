
#pragma once

#include <KAI/Language/Common/Structure.h>
#include <KAI/Language/Common/ParserCommon.h>
#include "KAI/Language/Tau/TauAstNode.h"
#include "KAI/Language/Tau/TauLexer.h"

KAI_BEGIN

// parser specific to the in-fix Tau language
class TauParser : public ParserCommon<TauLexer, TauAstEnumType>
{
public:
	typedef ParserCommon<TauLexer, TauAstEnumType> Parent;
	using typename Parent::TokenEnum;
	using typename Parent::TokenNode;
	using typename Parent::Lexer;
	using typename Parent::AstNode;
	using typename Parent::AstNodePtr;

	typedef TauAstEnumType NodeType;
	typedef TauTokenEnumType TokenType;

	TauParser(Registry &r) : Parent(r) { }

	virtual void Process(std::shared_ptr<Lexer> lex, Structure st) override;

protected:
	void Process(Structure);
};

KAI_END
