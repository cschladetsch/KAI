#pragma once

KAI_BEGIN

struct TranslatorCommon : Process
{
public:
	std::string Result() const;
	void PushNew();
	Pointer<Continuation> Pop();
	void Append(Object ob);
	Pointer<Continuation> Top();

	std::string Result() const;

	struct Exception { };
	struct Unsupported : Exception { };


private:
	std::vector<Pointer<Continuation>> stack;
	Registry &reg;
};

KAI_END


