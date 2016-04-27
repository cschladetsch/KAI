#pragma once

KAI_BEGIN

class Executor;

class Continuation : public Reflected
{
public:
	typedef Pointer</*const*/ Array> Code;

///private:
	Object scope;
	Pointer<Array> code;
	Pointer<Array> args;
	Pointer<String> source_code;
	Pointer<int> index;
	Pointer<bool> entered;

	// if true, this is a 'top-level' continuation, so
	// name resolution should stop here
	Pointer<bool> scopeBreak;

public:
	void Create();
	bool Destroy();

	void SetCode(Code);
	void SetCode(Code, String const *source);

	//void AddArg(Label const &arg) { args->Append(New(arg)); }
	void AddArg(Label const &arg);// { args->Append(New(arg)); }

	Code &GetCode() { return code; }
	const Code &GetCode() const { return code; }

	Pointer<String> GetSourceCode() const { return source_code; }
	//void SetSourceCode(const char *C) { source_code = New<String>(C); }
	void SetSourceCode(const char *C);// { source_code = New<String>(C); }

	void SetScope(Object const &Q) { scope = Q; }
	Object GetScope() const { return scope; }
	bool HasScope() const { return scope.Exists(); }

	bool HasCode() const { return code.Exists(); }

	int InitialStackDepth;

	void Enter(Executor *exec);

	// get next object in the continuation
	bool Next() const;
	bool Next(Object &) const;

	static void Register(Registry &);
};

StringStream &operator<<(StringStream &, const Continuation &);
StringStream &operator>>(StringStream &, Continuation &);
BinaryStream &operator<<(BinaryStream &, const Continuation &);
BinaryPacket &operator>>(BinaryPacket &, Continuation &);

KAI_TYPE_TRAITS(Continuation, Number::Continuation, Properties::Streaming | Properties::Reflected);

KAI_END

// TODO TESTS
//#include KAI_TEST(Continuation)
