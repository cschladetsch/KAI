
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_COMPILER_H
#	define KAI_COMPILER_H

KAI_BEGIN

struct Compiler;

StringStream &operator<<(StringStream &, Compiler const &);
BinaryStream &operator<<(BinaryStream &, Compiler const &);
BinaryPacket &operator>>(BinaryPacket &, Compiler &);

KAI_TYPE_TRAITS(Compiler, Type::Number::Compiler, Type::Properties::Reflected);

struct Compiler : Reflected<Compiler>
{
	struct Token
	{
		enum Type
		{
			None, StringLiteral, Word, OpenBlock, CloseBlock, OpenArray, CloseArray, 
			OpenList, CloseList, OpenMap, CloseMap, Comment,
		};

		String::const_iterator first;
		String::const_iterator second;
		Type type;

		Token(Type T = None) : type(T) { }
		Token(String::const_iterator F, String::const_iterator S, Type T = None) : first(F), second(S), type(T) { }
	};

	typedef nstd::vector<Token> Tokens;
	typedef nstd::map<Operation, String> OperationToString;
	typedef nstd::map<String, Pointer<Operation> > StringToOperation;

private:
	OperationToString op_to_string;
	StringToOperation string_to_op;

public:
	void Create();
	bool Destroy();

	Pointer<Continuation> Compile(Registry &, const String &) const;
	Object Compile(String const &) const;
	Object Compile2(String) const;

	static void Register(Registry &, const char * = "Compiler");

	void AddOperation(int N, const String &S);

	friend bool operator<(const Compiler &A, const Compiler &B);
	friend bool operator==(const Compiler &A, const Compiler &B);

protected:
	void Tokenise(const String &text, Tokens &tokens) const;
	Token NextToken(String::const_iterator A, String::const_iterator B) const;
	Pointer<Continuation> Parse(Registry &R, const Tokens &tokens) const;
	Pointer<Continuation> ParseContinuation(Registry &R, Tokens::const_iterator &A, Tokens::const_iterator B) const;
};

KAI_END

#include KAI_TEST(Compiler)

#endif // KAI_COMPILER_H

//EOF

