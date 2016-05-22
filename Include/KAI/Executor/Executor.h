#pragma once

#include <KAI/Core/Value.h>
#include <KAI/Core/Config/Base.h>
#include <KAI/Core/Type/TraitMacros.h>

#include <KAI/Executor/Continuation.h>
#include <KAI/Executor/Operation.h>
#include <KAI/Core/BuiltinTypes/Stack.h>

#include "KAI/Core/Object/Reflected.h"
#include "KAI/Core/Value.h"
#include "KAI/Core/Pathname.h"

KAI_BEGIN

class Tree;
class Executor;

KAI_TYPE_TRAITS(Executor, Number::Executor , Properties::Reflected); 

class Executor : public Reflected
{
	Value<Continuation> _continuation;
	Value<Stack> _context;
	Value<Stack> _data;
	Object _compiler;
	bool _break;
	Tree *_tree;
	int _traceLevel;
	int _stepNumber;

public:
	void Create();
	bool Destroy();

	void SetContinuation(Value<Continuation>);
	void Continue(Value<Continuation>);
	void ContinueOnly(Value<Continuation> C);	// continue C, do not call into the context stack after
	void ContinueTestCode(Value<Continuation> C);	// continue C, leaving one result on the stack
	void Continue();

	// just so we cn switch languages
	Object GetCompiler() const { return _compiler; }
	void SetCompiler(Object c) { _compiler = c; }

	void Eval(Object const &Q);
	void Dump(Object const &Q);
	
	std::string PrintStack() const;

	template <class T>
	Value<T> New()
	{
		return Reg().New<T>();
	}

	template <class T>
	Value<T> New(T const & X)
	{
		return Reg().New(X);
	}

	void SetTree(Tree *T) { _tree = T; }
	Tree *GetTree() const { return _tree; }

	void SetTraceLevel(int);
	int GetTraceLevel() const;

	template <class T>
	void Push(const Value<T> &val)
	{
		Push(val.GetObject());
	}
	
	void Push(Object const &);
	void Push(const std::pair<Object, Object> &);
	Object Pop();
	Object Top() const;

	Value<Stack> GetDataStack();
	Value<const Stack> GetDataStack() const
	{
		// TODO: automate
		return Value<const Stack>(_data.GetConstObject());
	}

	Value<const Stack> GetContextStack() const;

	/// This resets the entire process, other than static state stored
	/// in referenced objects
	void ClearStacks()
	{
		_data->Clear();
		_context->Clear();
	}

	static void Register(Registry &, const char * = "Executor");

	friend bool operator<(const Executor &A, const Executor &B);
	friend bool operator==(const Executor &A, const Executor &B);

	void ClearContext();
	
	void DropN();

	Object ResolvePop();
	Object Resolve(Object) const;
	Object Resolve(const Label &) const;
	Object Resolve(const Pathname &) const;

protected:
	void Perform(Operation::Type op);
	void ToArray();

	void GetChildren();
	void Expand();
	void MarkAndSweep();
	void MarkAndSweep(Object &root);

	void Push(Stack& L, Object const &Q);
	Object Pop(Stack &stack);
	void NextContinuation();
	
	void DumpStack(Stack const &);
	static void DumpContinuation(Continuation const &, int);

private:
	template <class C>
	Value<Array> ForEach(C const &, Object const &);
	template <class Cont>
	void PushAll(const Cont &cont);

	void TraceAll();
	void Trace(const Object &);
	void Trace(const Label &, const StorageBase&, StringStream &);
	void Trace(const Object&, StringStream &);
	void ConditionalContextSwitch(Operation::Type);
	Value<Continuation> NewContinuation(Value<Continuation> P);
	Object TryResolve(Label const &label) const;
};

StringStream &operator<<(StringStream &, Executor const &);
BinaryStream &operator<<(BinaryStream &, Executor const &);
BinaryPacket &operator>>(BinaryPacket &, Executor &);

KAI_END

