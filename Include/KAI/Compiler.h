#pragma once

#include <string>
#include <fstream>
#include <streambuf>

#include "KAI/ExecutorPCH.h"
#include "KAI/Translator/Base.h"
#include "KAI/Translator/Common.h"
#include "KAI/Translator/Structure.h"
#include "KAI/Translator/Language.h"
//#include "KAI/Translator/Pi/PiTranslator.h"
//#include "KAI/Translator/Rho/RhoTranslator.h"

KAI_BEGIN

class Compiler;

StringStream &operator<<(StringStream &, Compiler const &);
BinaryStream &operator<<(BinaryStream &, Compiler const &);
BinaryPacket &operator>>(BinaryPacket &, Compiler &);

KAI_TYPE_TRAITS(Compiler, Number::Compiler, Properties::Reflected);

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

	Pointer<Continuation> Compiler::Translate(const String &text, Structure st = Structure::Statement) const
	{
		switch (_language)
		{
		case Language::None:
			return Object();
		case Language::Pi:
			return Compile<PiTranslator>(text, st);
		case Language::Rho:
			return Compile<RhoTranslator>(text, st);
			return Object();
		}
		return Object();
	}

	template <class Trans>
	Pointer<Continuation> Compiler::Compile(const String &text, Structure st) const
	{
		if (text.empty())
			return Object();

		std::shared_ptr<Trans> p = std::make_shared<Trans>(Reg());
		p->Translate(text.c_str(), st);
		if (p->Failed)
		{
			std::cerr << p->Error;
			return Object();
		}

		return p->Top();
	}

	Pointer<Continuation> Compiler::CompileFile(const String &fileName, Structure st) const
	{
		std::ifstream t(fileName.c_str());
		std::stringstream buffer;
		buffer << t.rdbuf();
		return Translate(buffer.str().c_str(), st);
	}

	static void Register(Registry &, const char * = "Compiler");

	void AddOperation(int N, const String &S);

	//friend bool operator<(const Compiler &A, const Compiler &B);
	//friend bool operator==(const Compiler &A, const Compiler &B);
};

KAI_END
