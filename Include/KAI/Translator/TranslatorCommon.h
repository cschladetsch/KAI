#pragma once

KAI_BEGIN

struct TranslatorCommon : Process
{
public:
	std::string ToString() const;

	void PushNew();
	void Append(Object ob);

	Pointer<Continuation> Pop();
	Pointer<Continuation> Top() const;
	Pointer<Continuation> Result() const { return Top(); }

	struct Exception { };
	struct Unsupported : Exception { };

private:
	std::vector<Pointer<Continuation>> stack;
	Registry &reg;
};

KAI_END


