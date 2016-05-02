#pragma once

#include "KAI/KAICommon.h"
#include "KAI/Translator/Pi/PiToken.h"
#include "KAI/Translator/Pi/PiLexer.h"
#include "KAI/Translator/Pi/PiAstNode.h"
#include "KAI/Translator/Pi/PiParser.h"

KAI_BEGIN

struct PiTranslator : Process
{
	typedef PiAstNodeEnumType::Node AstNode;
	typedef PiToken Token;
	typedef LexerCommon<Token> Lexer;
	typedef ParserCommon<Lexer, AstNode> Parser;
	typedef typename PiParser::AstNodePtr AstNodePtr;
	typedef typename AstNode::Enum AstEnum;
	typedef PiTokenEnumType TokenType;
	typedef PiAstNodeEnumType AstType;

	PiTranslator(const PiTranslator&) = delete;
	PiTranslator(std::shared_ptr<PiParser> p, Registry &reg);

	struct Exception { };
	struct Unsupported : Exception { };

	std::vector<Pointer<Continuation>> stack;
	Registry &reg;

	std::string Result() const;

private:
	void Traverse(AstNodePtr node);
	void TranslateFunction(AstNodePtr node);
	void TranslateBlock(AstNodePtr node);
	void Translate(AstNodePtr node);
	void TranslateBinaryOp(AstNodePtr node, Operation::Type);

	// TODO: not create strings on each call: store the conversion in a map or something
	std::string ConvertOp(AstNodePtr node);

	void TranslateFromToken(AstNodePtr node);
	void TranslateCall(AstNodePtr node);
	void TranslateIndex(AstNodePtr node);

	Pointer<Continuation> Top();
	void PushNew();
	void Append(Object ob);
	template <class T>
	void AppendNew(T const &val)
	{
		Append(reg.New<T>(val));
	}
	template <class T>
	void AppendNew()
	{
		Append(reg.New<T>());
	}
	Pointer<Continuation> Pop();
	void AppendNewOp(Operation::Type op);
	void TranslateIf(AstNodePtr node);
	void TranslateFor(AstNodePtr node);
	void TranslateWhile(AstNodePtr node);
};


KAI_END