#pragma once

#include "KAI/Translator/Structure.h"
#include "KAI/Translator/Rho/RhoLang.h"

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

public:
	void Create();
	bool Destroy();

	Pointer<Continuation> Compile(Registry &, const String &, Structure = Structure::Statement) const;
	Pointer<Continuation> CompileFile(Registry &, const String &, Structure = Structure::Program) const;

	static void Register(Registry &, const char * = "Compiler");

	void AddOperation(int N, const String &S);

	friend bool operator<(const Compiler &A, const Compiler &B);
	friend bool operator==(const Compiler &A, const Compiler &B);
};

KAI_END

// TODO TEST
//#include KAI_TEST(Compiler)
