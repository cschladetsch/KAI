#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/TranslatorCommon.h"

KAI_BEGIN

TranslatorCommon::TranslatorCommon(Registry &r)
	: reg(r)
{
}

void TranslatorCommon::Append(Object const &ob)
{
	Top()->GetCode()->Append(ob);
}

void TranslatorCommon::AppendOp(Operation::Type op)
{
	AppendNew(Operation(op));
}

Pointer<Continuation> TranslatorCommon::Top()
{
	return stack.back();
}

void TranslatorCommon::PushNew()
{
	Pointer<Continuation> c = reg.New<Continuation>();
	c->SetCode(reg.New<Array>());
	stack.push_back(c);
}

Pointer<Continuation> TranslatorCommon::Pop()
{
	auto top = Top();
	stack.pop_back();
	return top;
}

std::string TranslatorCommon::ToString() const
{
	StringStream str;
	for (auto ob : *stack.back()->GetCode())
		str << ' ' << ob;
	return str.ToString().c_str();
}

KAI_END

