#pragma once

KAI_BEGIN

struct TranslatorCommon : CommonBase
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
		Append(reg.New<T>(val));
	}

	template <class T>
	Value<T> NewObject()
	{
		return reg.New<T>();
	}

	template <class T>
	Value<T> NewObject(const T &val)
	{
		return reg.New<T>(val);
	}

	template <>
	void AppendNew<Operation::Type>(Operation::Type op)
	{
		AppendOp(op);
	}

	void AppendOp(Operation::Type op);

	struct Exception { };
	struct Unsupported : Exception { };

protected:
	std::vector<Pointer<Continuation> > stack;
};

KAI_END
