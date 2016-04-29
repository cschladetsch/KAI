#pragma once

#include <string>
#include <strstream>

#include "KAI/KAICommon.h"

#include "KAI/Translator/Process.h"
#include "KAI/Translator/RhoLang.h"
#include "KAI/Translator/Parser.h"

#include "KAI/Operation.h"
#include "KAI/BuiltinTypes/Array.h"
#include "KAI/Continuation.h"

KAI_BEGIN

struct Translator : Process//, boost::noncopyable
{
	Translator(const Translator&) = delete;
	Translator(std::shared_ptr<Parser> p, Registry &reg);

	struct Exception { };
	struct Unsupported : Exception { };

	std::vector<Pointer<Continuation>> stack;
	Registry &reg;

	std::string Result() const;

private:
	typedef Parser::NodePtr NodePtr;

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
	void TranslateIf(Parser::NodePtr node);
	void TranslateFor(Parser::NodePtr node);
	void TranslateWhile(Parser::NodePtr node);
};

KAI_END
