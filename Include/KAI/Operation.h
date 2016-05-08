#pragma once

KAI_BEGIN

// An Operation represents an action to be taken by and Executor
struct Operation
{
	enum Type
	{
		Return,
		SuspendNew,
		None,
		True, False,
		Equiv, NotEquiv, Less, Greater, LessOrEquiv, GreaterOrEquiv,
		Suspend, Replace, Resume,
		CppFunctionCall,
		CppMethodCall,
		Plus, Minus, Multiply, Divide, Modulo,
		TypeOf,
		Store, Retreive, Remove,
		New,
		Assign,
		ChangeScope,
		GetChildren,
		Break,
		Drop, DropN, Swap, Dup, Rot, RotN, Pick, Clear, Depth, Over,
		ToArray, ToList, ToMap, ToSet, ToHashMap, ToPair, ToVector2, ToVector3, ToVector4, Expand,
		Name, Fullname,

		LogicalNot, LogicalAnd, LogicalOr, LogicalXor, LogicalNand,
		BitwiseNot, BitwiseAnd, BitwiseOr, BitwiseXor, BitwiseNand,
		Lookup,
		TraceAll,
		Trace,
		This,
		Ref,
		Detach,
		Delete,
		Exists,
		Pin,
		Unpin,
		ContinuationBegin, ContinuationEnd,
		MarkAndSweep,
		ThisContext,
		ThisContinuation,

		If,
		IfElse,
		ForEachContained,

		IfThenSuspend,
		IfThenReplace,
		IfThenResume,

		IfThenSuspendElseSuspend,
		IfThenReplaceElseSuspend,
		IfThenResumeElseSuspend,

		IfThenSuspendElseReplace,
		IfThenReplaceElseReplace,
		IfThenResumeElseReplace,

		IfThenSuspendElseResume,
		IfThenReplaceElseResume,
		IfThenResumeElseResume,

		Executor,
		ExecFile,

		Assert,
		NTimes,
		ForEach,

		GetProperty,
		SetProperty,

		Index,
		PreInc,
		PreDec,
		PostInc,
		PostDec,

		WhileLoop,
		ForLoop,
		DoLoop,

		LevelStack,

		SetChild,
		GetChild,
		RemoveChild,

		SetManaged,

		PlusEquals,
		MinusEquals,
		MulEquals,
		DivEquals,
		ModEquals,

		ToPi,
		ToRho,
		StartPiSequence,
		StartRhoSequence,
		GarbageCollect,
	};

private:
	Type value;

public:
	Operation(int T = 0) : value(Type(T)) { }

	void SetType(Type T) { value = T; }
	Type GetTypeNumber() const { return value; }

	static const char *ToString(int);
	const char *ToString() const;

	friend bool operator<(const Operation &A, const Operation &B) { return A.value < B.value; }
	friend bool operator==(const Operation &A, const Operation &B) { return A.value == B.value; }
	friend bool operator!=(const Operation &A, const Operation &B) { return A.value != B.value; }
	
	static void Register(Registry &);
};

//KAI_TYPE_TRAITS(Operation, Number::Operation , Properties::Streaming);
KAI_TYPE_TRAITS(Operation, Number::Operation, 0);

KAI_END
