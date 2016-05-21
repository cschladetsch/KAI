#pragma once

#include <KAI/Language/Common/ProcessCommon.h>
#include <KAI/Language/Common/Structure.h>
#include <KAI/Executor/Continuation.h>
#include <KAI/Executor/Operation.h>

KAI_BEGIN

/// Common to all systems that translate from one grammar to another
struct TranslatorCommon : ProcessCommon
{
public:
	TranslatorCommon(Registry &r);

	virtual Pointer<Continuation> Translate(const char *text, Structure = Structure::Statement) = 0;

	std::string ToString() const;

	void PushNew();
	Pointer<Continuation> Pop();
	Pointer<Continuation> Top();
	Pointer<const Continuation> Top() const;
	virtual Pointer<Continuation> Result() { return Top(); }

	void Append(Object const &ob);

	template <class T>
	void AppendNew(T val)
	{
		Append(_reg.New<T>(val));
	}

	template <class T>
	Value<T> NewObject()
	{
		return _reg.New<T>();
	}

	template <class T>
	Value<T> NewObject(const T &val)
	{
		return _reg.New<T>(val);
	}

	/*
	template <>
	void AppendNew<Operation::Type>(Operation::Type op)
	{
		AppendOp(op);
	}
	*/

	void AppendOp(Operation::Type op);

	struct Exception { };
	struct Unsupported : Exception { };

protected:
	std::vector<Pointer<Continuation> > stack;
};

KAI_END
