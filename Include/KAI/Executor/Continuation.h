#pragma once

#include <KAI/Core/Config/Base.h>
#include <KAI/Core/Object/Reflected.h>
#include <KAI/Core/Pointer.h>
#include <KAI/Core/Object/Object.h>
#include <KAI/Core/Object/Label.h>
#include <KAI/Core/StringStream.h>
#include <KAI/Core/BinaryStream.h>

#include <KAI/Core/Type/Number.h>
#include <KAI/Core/Type/Properties.h>
#include <KAI/Core/Type/TraitMacros.h>
#include <KAI/Core/Type/Traits.h>

#include <KAI/Core/BuiltinTypes/Array.h>
#include <KAI/Core/BuiltinTypes/String.h>
#include <KAI/Core/Registry.h>
#include <KAI/Core/Object/Reflected.h>

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

	template <class T>
	Pointer<T> New() const
	{
		return Self->GetRegistry()->New<T>();
	}

	template <class T>
	Pointer<T> New(const T &val) const
	{
		return Self->GetRegistry()->New<T>(val);
	}

	void SetCode(Code);
	void SetCode(Code, String const *source);

	Pointer<const Array> GetCode() const { return code; }

	void AddArg(Label const &arg) { args->Append(New(arg)); }

	Code &GetCode() { return code; }

	Pointer<String> GetSourceCode() const { return source_code; }
	void SetSourceCode(const char *C);

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
