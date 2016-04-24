
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_COMPILER_H
#	define KAI_COMPILER_H

#include "KAI/Translator/RhoLang.h"

KAI_BEGIN

struct Compiler;

StringStream &operator<<(StringStream &, Compiler const &);
BinaryStream &operator<<(BinaryStream &, Compiler const &);
BinaryPacket &operator>>(BinaryPacket &, Compiler &);

KAI_TYPE_TRAITS(Compiler, Number::Compiler, Properties::Reflected);

struct Compiler : Reflected//<Compiler>
{
	typedef nstd::map<Operation, String> OperationToString;
	typedef nstd::map<String, Pointer<Operation> > StringToOperation;

private:
	OperationToString op_to_string;
	StringToOperation string_to_op;

public:
	void Create();
	bool Destroy();

	Pointer<Continuation> Compile(Registry &, const String &, Parser::Structure = Parser::ParseStatement) const;
	Pointer<Continuation> CompileFile(Registry &, const String &, Parser::Structure = Parser::ParseProgram) const;

	static void Register(Registry &, const char * = "Compiler");

	void AddOperation(int N, const String &S);

	friend bool operator<(const Compiler &A, const Compiler &B);
	friend bool operator==(const Compiler &A, const Compiler &B);
};

KAI_END

#include KAI_TEST(Compiler)

#endif // KAI_COMPILER_H

//EOF

