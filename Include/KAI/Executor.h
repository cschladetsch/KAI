#pragma once
#include <KAI/Value.h>

KAI_BEGIN

class Tree;
class Executor;

KAI_TYPE_TRAITS(Executor, Number::Executor , Properties::Reflected); 

class Executor : public Reflected
{
	Value<Continuation> continuation;
	Value<Stack> context;
	Value<Stack> data;
	bool Break;
	Tree *tree;
	int traceLevel;
	int stepNumber;

public:
	void Create();
	bool Destroy();

	void SetContinuation(Value<Continuation>);
	void Continue(Value<Continuation>);
	void ContinueOnly(Value<Continuation> C);	// continue C, do not call into the context stack after
	void ContinueTestCode(Value<Continuation> C);	// continue C, leaving one result on the stack
	void Continue();

	void Eval(Object const &Q);
	void Dump(Object const &Q);

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

	void SetTree(Tree *T) { tree = T; }
	Tree *GetTree() const { return tree; }

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
	Value<const Stack> GetDataStack() const { return Value<const Stack>(data.GetConstObject()); }	// TODO: automate
	Value<const Stack> GetContextStack() const;
	void ClearStacks() { data->Clear(); context->Clear(); }

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
