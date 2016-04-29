#include "KAI/ExecutorPCH.h"
//#include "KAI/KAIRegistry.h"
//#include "KAI/Operation.h"
//#include "KAI/BuiltinTypes/All.h"
//#include "KAI/Continuation.h"
//#include "KAI/Value.h"
//#include "KAI/Compiler.h"
//#include "KAI/Reflected.h"
//#include "KAI/GetStorageBase.h"
//#include "KAI/Executor.h"
//#include "KAI/Pathname.h"
//#include "KAI/Tree.h"
//#include "KAI/Function.h"
//#include "KAI/SignedContinuation.h"
//#include "KAI/BuiltinTypes/List.h"
//#include "KAI/StringStream.h"

#undef GetObject

#include <iostream>

using namespace std;

KAI_BEGIN

const ClassBase *GetClass(Object const &Q);

void Executor::Create()
{
	data = New<Stack>();
	context = New<Stack>();
	Break = false;
	traceLevel = 0;
	stepNumber = 0;
}

bool Executor::Destroy()
{
	return true;
}

void Executor::Push(Object const &Q)
{
	if (Q.GetTypeNumber() == Type::Number::Object)
		Push(*data, ConstDeref<Object>(Q));
	else
		Push(*data, Q);
}

void Executor::Push(const std::pair<Object, Object> &P)
{
	Push(New(Pair(P.first, P.second)).GetObject());
}

Object Executor::ResolvePop()
{
	return Resolve(Pop(*data));
}

Object Executor::Pop()
{
	return Pop(*data);
}

Value<Stack> Executor::GetDataStack()
{
	return data;
}

Value<const Stack> Executor::GetContextStack() const
{
	return Value<const Stack>(data.GetConstObject()); // TODO: automatereturn context;
}


void Executor::SetContinuation(Value<Continuation> C)
{
	continuation = C;
	if (continuation.Exists())
	{
		continuation->Enter(this);
		continuation->InitialStackDepth = data->Size();
	}
}

struct Trace
{
	static std::ostream &Debug()
	{
		return std::cout;
	}
};

void Executor::Continue()
{
	Object next;
	for (;;)
	{
		Break = false;
		if (continuation->Next(next))
		{
			KAI_TRY
			{
				Eval(next);
			}
			KAI_CATCH(Exception::Base, E)
			{
				KAI_TRACE_3(data, context, continuation);
				KAI_TRACE_1(E);
				throw;
			}
		}
		else
			Break = true;

		if (Break)
		{
			NextContinuation();
			if (!continuation.Exists())
				return;
		}
	}
}

void Executor::Continue(Value<Continuation> C)
{
	SetContinuation(C);
	Continue();
}

void Executor::NextContinuation()
{
	if (context->Empty())
	{
		continuation = Object();
		return;
	}

	if (context->Empty())
	{
		KAI_TRACE_ERROR() << "Context stack is empty";
		KAI_NOT_IMPLEMENTED();
	}

	SetContinuation(context->Pop());
}

void Executor::Push(Stack& L, Object const &Q)
{
	L.Push(Q);
}

void Executor::Eval(Object const &Q)
{
	Dump(Q);
	stepNumber++;

	switch (GetTypeNumber(Q).value)
	{
	case Type::Number::Operation:
		Perform(Deref<Operation>(Q).GetTypeNumber()); 
		break;

	case Type::Number::Pathname:
		Push(Q);
		break;

	case Type::Number::Label:
		{
			auto found = TryResolve(ConstDeref<Label>(Q));
			if (found)
				Push(found);
			else
				Push(Q);
		}
		break;

	default:
		Push(Q.Clone());
		break;
	}
}

template <class Cont>
void Executor::PushAll(const Cont &cont)
{
	typename Cont::const_iterator A = cont.Begin(), B = cont.End();
	for (; A != B; ++A)
		Push(*A);

	Push(New(cont.Size()));
}

Value<Continuation> Executor::NewContinuation(Value<Continuation> orig)
{
	Value<Continuation> cont = New<Continuation>();
	cont->SetCode(orig->GetCode());
	cont->args = orig->args;
	return cont;
}

void Executor::MarkAndSweep()
{
	Object Q = tree->GetRoot();
	MarkAndSweep(Q);
}

void Executor::MarkAndSweep(Object &root)
{
	Registry &R = *root.GetRegistry();
	R.GarbageCollect();
}

void Executor::Expand()
{
	Object Q = Pop();
	switch (Q.GetTypeNumber().value)
	{
	case Type::Number::Pair:
		{
			const Pair &P = ConstDeref<Pair>(Q);
			Push(P.first);
			Push(P.second);
		}
		break;

	case Type::Number::List: PushAll(ConstDeref<List>(Q)); 
		break;

	case Type::Number::Array: PushAll(ConstDeref<Array>(Q)); 
		break;

	case Type::Number::Map: PushAll(ConstDeref<Map>(Q)); 
		break;

	default:
		KAI_THROW_1(Base, "Invalid Expand target");
		break;
	}
}


void Executor::GetChildren()
{
	Value<Array> children = New<Array>();
	const StorageBase &Q = GetStorageBase(Pop());
	const Dictionary &D = Q.GetDictionary();
	Dictionary::const_iterator A = D.begin(), B = D.end();
	for (; A != B; ++A)
		children->Append(New(A->first.ToString()).GetObject());

	Push(children.GetObject());
}

void Executor::ToArray()
{
	int len = ConstDeref<int>(Pop());
	if (len < 0)
		KAI_THROW_1(BadIndex, len);

	Value<Array> A = New<Array>();
	while (len-- > 0)
		A->Append(Pop());

	Push(A.GetObject());
}

void Executor::DropN()
{
	int N = Deref<int>(Pop());
	if (N < 0)
		KAI_THROW_1(BadIndex, N);

	while (N-- > 0)
		Pop();
}

void Executor::ConditionalContextSwitch(Operation::Type op)
{
	if (!ConstDeref<bool>(Pop()))
	{
		Pop();
		return;
	}

	switch (op)
	{
	case Operation::Suspend:
		continuation->Next();
		context->Push(continuation.GetObject());
	case Operation::Replace:
		context->Push(NewContinuation(Pop()).GetObject());
	case Operation::Resume:
		Break = true;
		break;
	}
}

void Executor::ContinueOnly(Value<Continuation> C)
{
	context->Push(Object());	// add an empty context to break. this forces exection to stop after C is finished
	Continue(C);
}

void Executor::ContinueTestCode(Value<Continuation> test)
{
	// preserve the stack depth going into the test
	int start_depth = data->Size();

	// execute the test code
	ContinueOnly(test);

	// ensure we have at least one extra argument on the stack
	int new_depth = data->Size();
	if (new_depth < start_depth + 1)
		KAI_THROW_1(Base, "Corrupted stack");

	// pop off the extraneous left-overs from the conditional
	int num_pops = data->Size() - new_depth - 1;
	for (int N = 0; N < num_pops; ++N)
		data->Pop();
}

void Executor::Perform(Operation::Type op)
{
	switch (op)
	{
	case Operation::SuspendNew:
		{
			Object what = ResolvePop();
			switch (what.GetTypeNumber().GetValue())
			{
			case Type::Number::Function:
				ConstDeref<BasePointer<FunctionBase> >(what)->Invoke(*what.GetRegistry(), *data);
				return;

			case Type::Number::Continuation:
				context->Push(continuation.GetObject());
				auto next = NewContinuation(what);
				*next->scopeBreak = true;
				context->Push(next.GetObject());
				Break = true;
				return;
			}

			break;
		}

	case Operation::Lookup:
		Push(Resolve(Pop()));
		break;

	case Operation::SetManaged:
		{
			Object object = Pop();
			bool managed = ConstDeref<bool>(Pop());
			object.SetManaged(managed);
		}
		break;

	case Operation::SetChild:
		{
			Pointer<Label> label = Pop();
			Object parent = Pop();
			Object child = Pop();
			parent.SetChild(*label, child);
		}
		break;

	case Operation::GetChild:
		{
			Pointer<Label> label = Pop();
			Object parent = Pop();
			Push(parent.GetChild(*label));
		}
		break;

	case Operation::RemoveChild:
		{
			Pointer<Label> label = Pop();
			Object parent = Pop();
			parent.RemoveChild(*label);
		}
		break;

	case Operation::ToVector2:
		{
			Pointer<float> y = Pop();
			Pointer<float> x = Pop();
			Value<Vector2> V = New<Vector2>();
			V->x = *x;
			V->y = *y;
			Push(V);
		}
		break;

	case Operation::ToVector3:
		{
			Value<float> z = Pop();
			Value<float> y = Pop();
			Value<float> x = Pop();
			Value<Vector3> V = New<Vector3>();
			V->x = *x;
			V->y = *y;
			V->z = *z;
			Push(V.GetObject());
		}
		break;

	case Operation::ToVector4:
		{
			Value<float> w = Pop();
			Value<float> z = Pop();
			Value<float> y = Pop();
			Value<float> x = Pop();
			Value<Vector4> V = New<Vector4>();
			V->x = *x;
			V->y = *y;
			V->z = *z;
			V->w = *w;
			Push(V);
		}
		break;
	
	case Operation::LevelStack:
		{
			int required_depth = continuation->InitialStackDepth;
			int depth = data->Size();
			if (depth < required_depth)
				KAI_THROW_0(EmptyStack);	// we lost some objects off the stack
			int num_pops = depth - required_depth;
			for (int N = 0; N < num_pops; ++N)
				data->Pop();
		}
		break;
	
	case Operation::PostInc:
		{
			Value<int> N = Pop();
			Value<int> M = New<int>();
			int &ref = *N;
			*M = ref;
			++ref;
			Push(M.GetObject());
		}
		break;
	
	case Operation::PostDec:
		{
			Value<int> N = Pop();
			Value<int> M = New<int>();
			int &ref = *N;
			*M = ref;
			--ref;
			Push(M.GetObject());
		}
		break;
	
	case Operation::PreInc:
		{
			Pointer<int> N = Pop();
			++*N;
			Push(N);
		}
		break;
	
	case Operation::PreDec:
		{
			Pointer<int> N = Pop();
			--*N;
			Push(N);
		}
		break;
	
	case Operation::Break:
		//__asm { int 3 };
		break;
	
	case Operation::WhileLoop:
		{
			Pointer<Continuation> body = Pop();
			Pointer<Continuation> test = Pop();
			context->Push(continuation.GetObject());	// for scoping
			ContinueTestCode(test);
			while (Deref<bool>(Pop()))
			{
				ContinueOnly(body);
				ContinueTestCode(test);
			}

			context->Pop();
		}
		break;

	case Operation::ThisContinuation:
		Push(continuation.GetObject());
		break;

	case Operation::Delete:
		Pop().Delete();
		break;
	
	case Operation::GetProperty:
		{
			Label const &L = ConstDeref<Label>(Pop());
			Object Q = Pop();
			Push(Q.GetClass()->GetProperty(L).GetValue(Q));
		}
		break;
	
	case Operation::SetProperty:
		{
			Label const &L = ConstDeref<Label>(Pop());
			Object Q = Pop();
			Q.GetClass()->GetProperty(L).SetValue(Q, Pop());
		}
		break;
	
	case Operation::Suspend:
		{
			if (data->Size() < 1)
			{
				KAI_TRACE_ERROR() << "Suspend: nothing to suspend to";
				KAI_NOT_IMPLEMENTED();
			}
			Object where_to_go = Resolve(Pop());
			switch (where_to_go.GetTypeNumber().GetValue())
			{
			case Type::Number::Function:
				ConstDeref<BasePointer<FunctionBase> >(where_to_go)->Invoke(*where_to_go.GetRegistry(), *data);
				return;

			case Type::Number::SignedContinuation:
				{
					SignedContinuation &signed_continuation = Deref<SignedContinuation>(where_to_go);
					signed_continuation.Enter(*data);
					where_to_go = signed_continuation.GetContinuation();
				}
				break;

			case Type::Number::Continuation:
				break;
			}

			context->Push(continuation.GetObject());
			context->Push(where_to_go);
			if (where_to_go.IsType<Continuation>())
				Deref<Continuation>(where_to_go).Enter(this);

			Break = true;
		}
		break;

	case Operation::Return:
		{
			int n = 0;
			for (auto sc : *context)
			{
				if (*Deref<Continuation>(sc).scopeBreak)
					break;
				++n;
			}

			for (; n > 0; --n)
				context->Pop();
		}
		break;

	case Operation::Replace:
		context->Push(NewContinuation(Pop()).GetObject());
	case Operation::Resume:
		Break = true;
		break;

	case Operation::NTimes:
		{
			int M = ConstDeref<int>(Pop());
			if (M == 0)
				return;
			Pointer<Continuation> C = Pop();
			for (int N = 0; N < M; ++N)
			{
				// push a null continuation to break the call chain
				context->Push(Object());
				// re-continue the functor
				Continue(C);
			}
		}
		break;

	case Operation::ForEach:
		{
			Object F = Pop();
			Object C = Pop();
			switch (C.GetTypeNumber().ToInt())
			{
			case Type::Number::Array:
				Push(ForEach(ConstDeref<Array>(C), F));
				break;

			case Type::Number::Stack:
				Push(ForEach(ConstDeref<Stack>(C), F));
				break;

				/* TODO
			case Type::Number::List:
				ForEach(ConstDeref<List>(C), F);
				break;
			case Type::Number::Set:
				ForEach(ConstDeref<Set>(C), F);
				break;
				*/
			case Type::Number::Map:
				Push(ForEach(ConstDeref<Map>(C), F));
				break;

			case Type::Number::HashMap:
				Push(ForEach(ConstDeref<HashMap>(C), F));
				break;
			}
		}
		break;

	case Operation::Executor:
		Push(*Self);
		break;

	case Operation::ForEachContained:
		{
			Object gen = Pop();
			switch (gen.GetTypeNumber().ToInt())
			{
			case Type::Number::Set:
			case Type::Number::Map:
			case Type::Number::Array:
			case Type::Number::String:
				break;
			}

			KAI_NOT_IMPLEMENTED();
		}
		break;

	case Operation::If:
		if (!ConstDeref<bool>(Pop()))
			Pop();
		break;

	case Operation::IfElse:
		{
			if (data->Size() < 3)
			{
				KAI_TRACE_ERROR() << "attempting IfElse, but stack of " << data->Size() << " is too small";
				KAI_NOT_IMPLEMENTED();
			}

			Object condition = Pop();
			Object falseClause = Pop();
			Object trueClause = Pop();

			if (ConstDeref<bool>(condition))
				Push(trueClause);
			else
				Push(falseClause);
		}
		break;

	case Operation::IfThenSuspend:
		{
			Object then = Pop();
			if (ConstDeref<bool>(Pop()))
			{
				context->Push(continuation.GetObject());
				context->Push(NewContinuation(then).GetObject());
				Break = true;
			}
		}
		break;

	case Operation::IfThenSuspendElseSuspend:
		{
			Pointer<Continuation> else_ = Pop();
			Pointer<Continuation> then = Pop();
			context->Push(continuation.GetObject());
			if (ConstDeref<bool>(Pop()))
				context->Push(NewContinuation(then).GetObject());
			else
				context->Push(NewContinuation(else_).GetObject());

			Break = true;
		}
		break;

	case Operation::IfThenReplace:
		ConditionalContextSwitch(Operation::Replace);
		break;

	case Operation::IfThenResume:
		ConditionalContextSwitch(Operation::Resume);
		break;

	case Operation::Assign:
		{
			Object lhs = Pop();
			Object rhs = Pop();
			lhs.GetClass()->Assign(lhs.GetStorageBase(), rhs.GetStorageBase());
		}
		break;

	case Operation::ThisContext:
		Push(continuation.GetObject());
		break;

	case Operation::Remove:
		Remove(tree->GetRoot(), continuation->GetScope(), Pop());
		break;

	case Operation::MarkAndSweep:
		MarkAndSweep();
		break;

	case Operation::DropN:
		DropN();
		break;

	case Operation::Over:
		{
			Object A = Pop();
			Object B = Pop();
			Push(B);
			Push(A);
			Push(B);
		}
		break;

	case Operation::True:
		Push(New(true));
		break;

	case Operation::False:
		Push(New(false));
		break;

	case Operation::CppFunctionCall:
		{
			Object Q = Pop();
			ConstDeref<BasePointer<FunctionBase> >(Q)->Invoke(*Q.GetRegistry(), *data);
		}
		break;

	case Operation::Trace:
		{
			Object Q = Pop();
			Push(Q);
			Trace(Q);
		}
		break;

	case Operation::TraceAll:
		TraceAll();
		break;

	case Operation::Name:
		Push(New(GetName(Pop())));
		break;

	case Operation::Fullname:
		Push(New(GetFullname(Pop())));
		break;

	case Operation::New:
		{
			Object Q = Pop();
			switch (Q.GetTypeNumber().ToInt())
			{
			case Type::Number::String: Push(Self->GetRegistry()->NewFromClassName(ConstDeref<String>(Q).c_str())); 
				break;

			case Type::Number::TypeNumber: Push(Self->GetRegistry()->NewFromTypeNumber(ConstDeref<Type::Number>(Q))); 
				break;

			case Type::Number::Class: Push(Self->GetRegistry()->NewFromClass(ConstDeref<const ClassBase *>(Q))); 
				break;

			default:
				KAI_THROW_1(CannotNew, Q);
				break;
			}
		}
		break;
	case Operation::Assert:
		{
			if (!ConstDeref<bool>(Pop()))
				KAI_THROW_0(Assertion);
		}
		break;

	case Operation::Ref:
		Push(Top());
		break;

	case Operation::Drop:
		Pop();
		break;

	case Operation::Swap:
		{
			Object A = Pop();
			Object B = Pop();
			Push(A);
			Push(B);
		}
		break;

	case Operation::Dup:
		{
			Object Q = Pop();
			Push(Q);
			Push(Q.Duplicate());
		}
		break;

	case Operation::Rot:
		{
			Object A = Pop();
			Object B = Pop();
			Object C = Pop();
			Push(B);
			Push(A);
			Push(C);
		}
		break;

	case Operation::Clear:
		data->Clear();
		break;

	case Operation::Depth:
		Push(New(data->Size()));
		break;

	case Operation::ToPair:
		{
			Object B = Pop();
			Object A = Pop();
			Push(New(Pair(A, B)));
		}
		break;

	case Operation::ToArray:
		ToArray();
		break;

	case Operation::This:
		Push(continuation->GetScope());
		break;

	case Operation::Expand:
		Expand();
		break;

	case Operation::TypeOf:
		Push(New(Pop().GetClass()));
		break;

	case Operation::Exists:
		Push(New(Pop().Exists()));
		break;

	case Operation::GetChildren:
		GetChildren();
		break;

	case Operation::ChangeScope:
		{
			Object id = Pop();
			if (GetTypeNumber(id) == Type::Number::Label)
				tree->SetScope(GetStorageBase(tree->GetScope()).Get(ConstDeref<Label>(id)));
			else
				tree->SetScope(ConstDeref<Pathname>(id));
		}
		break;

	case Operation::PlusEquals:
		{
			Object arg = Pop();
			Object from = Pop();
			if (arg.IsType<float>() && from.IsType<float>())
			{
				Deref<float>(from) += ConstDeref<float>(arg);
				break;
			}

			if (arg.IsType<int>() && from.IsType<int>())
			{
				Deref<int>(from) += ConstDeref<int>(arg);
				break;
			}
			
			Object result = from.GetClass()->Plus(from, arg);
			from.GetClass()->Assign(from, result);
		}
		break;

	case Operation::MinusEquals:
		{
			Object arg = Pop();
			Object from = Pop();
			Object result = from.GetClass()->Minus(result, arg);
			from.GetClass()->Assign(from, result);
		}
		break;

	case Operation::MulEquals:
		{
			Object arg = Pop();
			Object from = Pop();
			Object result = from.GetClass()->Multiply(result, arg);
			from.GetClass()->Assign(from, result);
		}
		break;

	case Operation::DivEquals:
		{
			Object arg = Pop();
			Object from = Pop();
			Object result = from.GetClass()->Divide(result, arg);
			from.GetClass()->Assign(from, result);
		}
		break;

	case Operation::ModEquals:
		KAI_NOT_IMPLEMENTED();
		break;

	case Operation::Plus:
		{
			Object B = ResolvePop();
			Object A = ResolvePop();
			Push(*A.GetClass()->Plus(GetStorageBase(A), GetStorageBase(B)));
		}
		break;

	case Operation::Minus:
		{
			Object B = ResolvePop();
			Object A = ResolvePop();
			Push(*A.GetClass()->Minus(GetStorageBase(A), GetStorageBase(B)));
		}
		break;

	case Operation::Multiply:
		{
			Object B = ResolvePop();
			Object A = ResolvePop();
			Push(*A.GetClass()->Multiply(GetStorageBase(A), GetStorageBase(B)));
		}
		break;

	case Operation::Divide:
		{
			Object B = ResolvePop();
			Object A = ResolvePop();
			Push(*A.GetClass()->Divide(GetStorageBase(A), GetStorageBase(B)));
		}
		break;

	case Operation::Store:
		{
			Object ident = Pop();
			Object value = ResolvePop();
			if (!continuation->HasScope())
				continuation->SetScope(New<void>().GetObject());

			Set(tree->GetRoot(), continuation->GetScope(), ident, value);
		}
		break;

	case Operation::Retreive:
		Push(Resolve(Pop()));
		break;

	case Operation::Less:
		{
			Object B = ResolvePop();
			Object A = ResolvePop();
			Push(New(A.GetClass()->Less2(A,B)));
		}
		break;

	case Operation::NotEquiv:
		{
			Object B = ResolvePop();
			Object A = ResolvePop();
			Push(New(!A.GetClass()->Equiv2(A,B)));
		}
		break;

	case Operation::Equiv:
		{
			Object B = ResolvePop();
			Object A = ResolvePop();
			Push(New(A.GetClass()->Equiv2(A,B)));
		}
		break;

	case Operation::Greater:
		{
			Object B = ResolvePop();
			Object A = ResolvePop();
			Push(New(A.GetClass()->Greater2(A,B)));
		}
		break;

	case Operation::CppMethodCall:
		{
			const Label &method_name = ConstDeref<Label>(Pop());
			Object object = Pop();
			if (!object.Exists())
				KAI_THROW_0(NullObject);

			const ClassBase *klass = object.GetClass();
			MethodBase *method = klass->GetMethod(method_name);
			if (method == 0)
				KAI_THROW_2(UnknownMethod, method_name.ToString(), klass->GetName().ToString());

			method->Invoke(object, *data);
		}
		break;

	case Operation::LogicalNot:
		Push(New(!ConstDeref<bool>(ResolvePop())));
		break;

	case Operation::LogicalAnd:
		{
			bool A = ConstDeref<bool>(ResolvePop());
			bool B = ConstDeref<bool>(ResolvePop());
			Push(New(A && B));
		}
		break;

	case Operation::LogicalOr:
		{
			bool A = ConstDeref<bool>(ResolvePop());
			bool B = ConstDeref<bool>(ResolvePop());
			Push(New(A || B));
		}
		break;

	case Operation::LogicalXor:
		{
			bool A = ConstDeref<bool>(ResolvePop());
			bool B = ConstDeref<bool>(ResolvePop());
			Push(New(A ^ B));
		}
		break;

#define OPERATION_NOT_IMPLEMENTED(Op) \
	case Operation::Op: \
		{ \
			KAI_NOT_IMPLEMENTED_1(#Op); \
		} \
		break;
	OPERATION_NOT_IMPLEMENTED(IfThenReplaceElseSuspend);
	OPERATION_NOT_IMPLEMENTED(IfThenResumeElseSuspend);
	OPERATION_NOT_IMPLEMENTED(IfThenSuspendElseReplace);
	OPERATION_NOT_IMPLEMENTED(IfThenReplaceElseReplace);
	OPERATION_NOT_IMPLEMENTED(IfThenResumeElseReplace);
	OPERATION_NOT_IMPLEMENTED(IfThenSuspendElseResume);
	OPERATION_NOT_IMPLEMENTED(IfThenReplaceElseResume);
	OPERATION_NOT_IMPLEMENTED(IfThenResumeElseResume);
	OPERATION_NOT_IMPLEMENTED(RotN);
	OPERATION_NOT_IMPLEMENTED(Pick);
	OPERATION_NOT_IMPLEMENTED(ToList);
	OPERATION_NOT_IMPLEMENTED(LessOrEquiv);
	OPERATION_NOT_IMPLEMENTED(GreaterOrEquiv);
	OPERATION_NOT_IMPLEMENTED(LogicalNand);
	OPERATION_NOT_IMPLEMENTED(BitwiseNot);
	OPERATION_NOT_IMPLEMENTED(BitwiseAnd);
	OPERATION_NOT_IMPLEMENTED(BitwiseOr);
	OPERATION_NOT_IMPLEMENTED(BitwiseXor);
	OPERATION_NOT_IMPLEMENTED(BitwiseNand);
	}
}

template <class D>
Value<Array> Executor::ForEach(D const &C, Object const &F)
{
	Value<Array> R = New<Array>();
	typename D::const_iterator A = C.Begin(), B = C.End();
	for (; A != B; ++A)
	{
		Push(*A);
		context->Push(Object());
		Continue(F);
		R->Append(Pop());
	}

	return R;
}

Object Executor::Pop(Stack &stack)
{
	return stack.Pop();
}

Object Executor::Top() const
{
	return data->Top();
}

Object Executor::Resolve(Object ident) const
{
	if (ident.IsType<Label>())
		return Resolve(ConstDeref<Label>(ident));

	if (ident.IsType<Pathname>())
		return Resolve(ConstDeref<Pathname>(ident));

	return ident;
}

Object Executor::TryResolve(Label const &label) const
{
	// search in current scope
	if (continuation.Exists())
	{
		Object scope = continuation->GetScope();
		if (scope && scope.Has(label))
			return scope.Get(label);
	}

	// search in parent scopes
	Stack const &scopes = *context;
	for (int N = 0; N < scopes.Size(); ++N)
	{
		Pointer<Continuation> cont = scopes.At(N);
		if (!cont.Exists())
			continue;
		Object scope = cont->GetScope();
		if (scope.HasChild(label))
			return scope.GetChild(label);
	}

	// finally, search the tree
	return tree->Resolve(label);
}

Object Executor::Resolve(Label const &label) const
{
	auto object = TryResolve(label);
	if (!object.Valid())
		KAI_THROW_1(CannotResolve, label);

	return object;
}

Object Executor::Resolve(Pathname const &path) const
{
	// if its not an absolute path, search up the continuation scopes
	if (!path.Absolute)
	{
		// search in current scope
		if (continuation.Exists() && Exists(continuation->GetScope(), path))
			return Get(continuation->GetScope(), path);

		// search in parent scopes
		Stack const &scopes = *context;
		for (int N = 0; N < scopes.Size(); ++N)
		{
			Pointer<Continuation> cont = scopes.At(N);
			if (!cont.Exists())
				continue;

			Object scope = cont->GetScope();
			if (Exists(scope, path))
				return Get(scope, path);
		}
	}

	// finally, search the tree
	Object Q = tree->Resolve(path);
	if (!Q.Valid())
		KAI_THROW_1(CannotResolve, path);
	return Q;
}

void Executor::Trace(const Label &, const StorageBase &Q, StringStream &S)
{
	S
		<< "Handle=" << Q.GetHandle().GetValue() << ": " 
		<< "Parent=" << Q.GetParentHandle().GetValue() << ": "
		<< "Fullname=" << GetFullname(Q) << ": " 
		<< "Type=" << Q.GetClass()->GetName() << ": " 
		<< "StrStrm='" << Q
		<< "'\n";
}

void Executor::Trace(const Object& Q, StringStream &S)
{
	if (!Q.Valid())
	{
		S << "INVALID_OBJECT";
		return;
	}

	Trace(GetName(Q), Q.GetStorageBase(), S);
}

void Executor::Trace(const Object &Q)
{
	StringStream S;
	Trace(Q, S);
	Push(New(S.ToString()));
}

void Executor::ClearContext()
{
	continuation = Object();
	context->Clear();
}

void Executor::TraceAll()
{
	StringStream debug;
	debug << "DEBUG: ";
	const Registry::Instances &I = Self->GetRegistry()->GetInstances();
	Registry::Instances::const_iterator A = I.begin(), B = I.end();
	for (; A != B; ++A)
	{
		try
		{
			if (A->second == 0)
				debug << "!!!null object in registry!!!";
			else
				Trace(*A->second, debug);
		}
		catch (Exception::Base &E)
		{
			debug << "TraceError :( " << E.ToString();
		}
		catch (std::exception &E)
		{
			debug << "TraceError std::exeption:( " << E.what();
		}
		catch (...)
		{
			debug << "TraceError unknown :( ";
		}
	}

	Push(New(debug.ToString()));
}

void Executor::DumpStack(Stack const &stack)
{
	StringStream result;
	Stack::const_iterator A = stack.Begin(), B = stack.End();
	for (int N = stack.Size() - 1; A != B; ++A, --N)
	{
		result << "[" << N << "] ";
		bool is_string = A->GetTypeNumber() == Type::Number::String;
		if (is_string)
			result << "\"";

		result << *A;
		if (is_string)
			result << "\"";

		if (A->GetTypeNumber() == Type::Number::Label)
			result << " = " << TryResolve(ConstDeref<Label>(*A));

		result << "\n";
	}

	Trace::Debug() << result.ToString().c_str();
}

void Executor::DumpContinuation(Continuation const &C, int ip)
{
	KAI_UNUSED_2(C, ip);
}

void Executor::SetTraceLevel(int N)
{
	traceLevel = N;
}

int Executor::GetTraceLevel() const
{
	return traceLevel;
}

void Executor::Register(Registry &R, const char * N)
{
	ClassBuilder<Executor>(R, N)
		// TODO
		//.Methods
		//	("SetTraceLevel", &Executor::SetTraceLevel)
		//	("GetTraceLevel", &Executor::GetTraceLevel)
		//.Properties
		//	("Continuation", &Executor::continuation)
		//	("Context", &Executor::context)
		//	("Data", &Executor::data)
		;
}

void Executor::Dump(Object const &Q)
{
	if (traceLevel > 0)
	{
		if (traceLevel > 1)
		{
			std::cout << "Stack:\n";
			DumpStack(*data);
		}

		if (traceLevel > 2)
		{
			std::cout << "Context:\n";
			for (auto c : *context)
			{
				StringStream str;
				str << c;
				cout << str.ToString().c_str() << endl;
			}
		}

		std::cout << "\n[" << stepNumber << "]: Eval: @" << *continuation->index << " " << Q.ToString().c_str() << "\n";//std::endl;
	}
}

KAI_END

//EOF
