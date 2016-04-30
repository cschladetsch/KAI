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
	typedef LexerCommon<RhoToken> Lexer;
	typedef ParserCommon<LexerCommon<RhoToken>, RhoAstNode> Parser;
	typedef typename RhoParser::NodePtr NodePtr;

	RhoTranslator(const RhoTranslator&) = delete;
	RhoTranslator(std::shared_ptr<RhoParser> p, Registry &reg);

	struct Exception { };
	struct Unsupported : Exception { };

	std::vector<Pointer<Continuation>> stack;
	Registry &reg;

	std::string Result() const;

private:
	void Traverse(NodePtr node);
	void TranslateFunction(NodePtr node);
	void TranslateBlock(NodePtr node);
	void Translate(NodePtr node);
	void TranslateBinaryOp(NodePtr node, Operation::Type);

	// TODO: not create strings on each call: store the conversion in a map or something
	std::string ConvertOp(NodePtr node);

	void TranslateFromToken(NodePtr node);
	void TranslateCall(NodePtr node);
	void TranslateIndex(NodePtr node);

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
	void TranslateIf(NodePtr node);
	void TranslateFor(NodePtr node);
	void TranslateWhile(NodePtr node);
};

KAI_END
