#pragma once

#include <KAI/Executor/Operation.h>
#include <KAI/Language/Common/Language.h>
#include <KAI/Language/Rho/RhoTranslator.h>
#include <KAI/Language/Pi/PiTranslator.h>
#include <KAI/Core/Debug.h>

KAI_BEGIN

class Compiler : public Reflected
{
	typedef std::map<Operation, String> OperationToString;
	typedef std::map<String, Pointer<Operation> > StringToOperation;

private:
	OperationToString op_to_string;
	StringToOperation string_to_op;
	Language _language;

public:
	void Create();
	bool Destroy();

	void SetLanguage(int);
	int GetLanguage() const;

	Pointer<Continuation> Translate(const String &text, Structure st = Structure::Statement) const
	{
		switch (_language)
		{
		case Language::None:
			return Object();
		case Language::Pi:
			return Compile<PiTranslator>(text, st)->GetCode()->At(0);	// Pi wraps output in a continuation. TODO: fix
		case Language::Rho:
			return Compile<RhoTranslator>(text, st);
		case Language::Tau:
			KAI_NOT_IMPLEMENTED();
			break;
		}
		return Object();
	}

	template <class Trans>
	Pointer<Continuation> Compile(const String &text, Structure st = Structure::Expression) const
	{
		if (text.empty())
			return Object();

		auto p = std::make_shared<Trans>(Reg());
		auto result = p->Translate(text.c_str(), st);
		if (p->Failed)
		{
			KAI_TRACE_ERROR_1(p->Error);//, st);
			return Object();
		}

		return result;
	}

	Pointer<Continuation> CompileFile(const String &fileName, Structure st = Structure::Program) const;


	static void Register(Registry &, const char * = "Compiler");

	void AddOperation(int N, const String &S);
};

KAI_TYPE_TRAITS(Compiler, Number::Compiler, Properties::Reflected);

KAI_END
