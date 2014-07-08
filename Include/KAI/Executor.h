
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_EXECUTOR_H
#	define KAI_EXECUTOR_H

KAI_BEGIN

struct Tree;
class Executor;

KAI_TYPE_TRAITS(Executor, Type::Number::Executor
	, Type::Properties::Reflected); 

class Executor : public Reflected<Executor>
{
	Pointer<Continuation> continuation;
	Pointer<Stack> context;
	Pointer<Stack> data;
	bool Break;
	Tree *tree;
	int trace_level;

public:
	void Create();
	bool Destroy();

	void SetContinuation(Pointer<Continuation>);
	void Continue(Pointer<Continuation>);
	void ContinueOnly(Pointer<Continuation> C);	// continue C, do not call into the context stack after
	void ContinueTestCode(Pointer<Continuation> C);	// continue C, leaving one result on the stack
	void Continue();

	void Eval(Object const &Q);
	
	void SetTree(Tree *T) { tree = T; }
	Tree *GetTree() const { return tree; }

	void SetTraceLevel(int);
	int GetTraceLevel() const;

	void Push(Object const &);
	void Push(const nstd::pair<Object, Object> &);
	Object Pop();
	Object Top() const;

	Pointer<Stack> GetDataStack();
	Pointer<const Stack> GetDataStack() const { return data; }
	Pointer<const Stack> GetContextStack() const;
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
	Pointer<Array> ForEach(C const &, Object const &);
	template <class Cont>
	void PushAll(const Cont &cont);

	void TraceAll();
	void Trace(const Object &);
	void Trace(const Label &, const StorageBase&, StringStream &);
	void Trace(const Object&, StringStream &);
	void ConditionalContextSwitch(Operation::Type);
	Pointer<Continuation> NewContinuation(Pointer<Continuation> P);
	Object TryResolve(Label const &label) const;
};

StringStream &operator<<(StringStream &, Executor const &);
BinaryStream &operator<<(BinaryStream &, Executor const &);
BinaryPacket &operator>>(BinaryPacket &, Executor &);

KAI_END

#endif // KAI_EXECUTOR_H

//EOF
