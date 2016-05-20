#pragma once

#include <map>
#include <fstream>
#include <sstream>

#include <KAI/Core/BuiltinTypes/String.h>
#include <KAI/Executor/Operation.h>
#include <KAI/Language/Common/Language.h>
#include <KAI/Executor/Continuation.h>
#include <KAI/Language/Common/Structure.h>
#include <KAI/Core/Exception/Extended.h>

#include "KAI/Core/StringStream.h"
#include "KAI/Core/BinaryStream.h"
#include "KAI/Core/Debug.h"

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

	Pointer<Continuation> Translate(const String &text, Structure st = Structure::Statement) const
	{
		switch (_language)
		{
		case Language::None:
			return Object();
		case Language::Pi:
			return Compile<PiTranslator>(text, st);
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

		std::shared_ptr<Trans> p = std::make_shared<Trans>(Reg());
		auto result = p->Translate(text.c_str(), st);
		if (p->Failed)
		{
			//std::cerr << p->Error;
			KAI_TRACE_ERROR_1(p->Error);
			return Object();
		}

		return result;
	}

	Pointer<Continuation> CompileFile(const String &fileName, Structure st = Structure::Program) const
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
