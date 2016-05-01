#pragma once

#include "KAI/KAICommon.h"
#include "KAI/Operation.h"
#include "KAI/BuiltinTypes/Array.h"
#include "KAI/Continuation.h"

#include "KAI/Translator/Process.h"
#include "KAI/Translator/LexerCommon.h"
#include "KAI/Translator/ParserCommon.h"

#include "KAI/Translator/Rho/RhoToken.h"
#include "KAI/Translator/Rho/RhoAstNode.h"
#include "KAI/Translator/Rho/RhoParser.h"
#include "KAI/Translator/Rho/RhoLang.h"

KAI_BEGIN

struct RhoTranslator : Process
{
	typedef RhoAstNodeEnumType::Node AstNode;
	typedef RhoToken Token;
	typedef LexerCommon<Token> Lexer;
	typedef ParserCommon<Lexer, AstNode> Parser;
	typedef typename RhoParser::AstNodePtr AstNodePtr;
	typedef typename AstNode::Enum AstEnum;
	typedef RhoTokenEnumType TokenType;
	typedef RhoAstNodeEnumType AstType;

	RhoTranslator(const RhoTranslator&) = delete;
	RhoTranslator(std::shared_ptr<RhoParser> p, Registry &reg);

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
