#pragma once

#include <string>
#include <strstream>

#include "KAI/Translator/Parser.h"

#include "KAI/KAI.h"
#include "KAI/Operation.h"
#include "KAI/BuiltinTypes/Array.h"
#include "KAI/Continuation.h"

//#define SCHLADETSCH_NAMESPACE KAI_NAMESPACE_NAME
//#include <EventP.h>

KAI_BEGIN

struct Translator : Process, boost::noncopyable
{
	struct Exception { };
	struct Unsupported : Exception { };

	std::vector<Pointer<Continuation>> stack;
	Registry &reg;

	Translator(std::shared_ptr<Parser> p, Registry &reg);

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
};

KAI_END
