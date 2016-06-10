// only needed for code introspection
#include <KAI/Executor/Executor.h>

void Executor::Perform(Operation::Type op)
{
	switch (op)
	{
	case Operation::ToPi:
		Deref<Compiler>(_compiler).SetLanguage((int)Language::Pi);
		break;

	case Operation::ToRho:
		Deref<Compiler>(_compiler).SetLanguage((int)Language::Rho);
		break;

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
		int required_depth = _continuation->InitialStackDepth;
		int depth = _data->Size();
		if (depth < required_depth)
			KAI_THROW_0(EmptyStack);	// we lost some objects off the stack
		int num_pops = depth - required_depth;
		for (int N = 0; N < num_pops; ++N)
			_data->Pop();
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
		_context->Push(_continuation.GetObject());	// for scoping
		ContinueTestCode(test);
		while (Deref<bool>(Pop()))
		{
			ContinueOnly(body);
			ContinueTestCode(test);
		}

		_context->Pop();
	}
		break;

	case Operation::ThisContinuation:
		Push(_continuation.GetObject());
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
		if (_data->Size() < 1)
		{
			KAI_TRACE_ERROR() << "Suspend: nothing to suspend to";
			KAI_NOT_IMPLEMENTED();
		}
		Object where_to_go = Resolve(Pop());
		switch (where_to_go.GetTypeNumber().GetValue())
		{
		case Type::Number::Function:
			ConstDeref<BasePointer<FunctionBase> >(where_to_go)->Invoke(*where_to_go.GetRegistry(), *_data);
			return;

		case Type::Number::SignedContinuation:
		{
			SignedContinuation &signed_continuation = Deref<SignedContinuation>(where_to_go);
			signed_continuation.Enter(*_data);
			where_to_go = signed_continuation.GetContinuation();
		}
			break;

		case Type::Number::Continuation:
			break;
		}

		_context->Push(_continuation.GetObject());
		_context->Push(where_to_go);
		if (where_to_go.IsType<Continuation>())
			Deref<Continuation>(where_to_go).Enter(this);

		_break = true;
	}
		break;

	case Operation::Return:
	{
		int n = 0;
		for (auto sc : *_context)
		{
			if (*Deref<Continuation>(sc).scopeBreak)
				break;
			++n;
		}

		for (; n > 0; --n)
			_context->Pop();
	}
		break;

	case Operation::Replace:
		_context->Push(NewContinuation(Pop()).GetObject());
	case Operation::Resume:
		_break = true;
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
			_context->Push(Object());
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

			/* TODO
			case Type::Number::HashMap:
				Push(ForEach(ConstDeref<HashMap>(C), F));
				break;
				*/
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
		if (_data->Size() < 3)
		{
			KAI_TRACE_ERROR() << "attempting IfElse, but stack of " << _data->Size() << " is too small";
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
			_context->Push(_continuation.GetObject());
			_context->Push(NewContinuation(then).GetObject());
			_break = true;
		}
	}
		break;

	case Operation::IfThenSuspendElseSuspend:
	{
		Pointer<Continuation> else_ = Pop();
		Pointer<Continuation> then = Pop();
		_context->Push(_continuation.GetObject());
		if (ConstDeref<bool>(Pop()))
			_context->Push(NewContinuation(then).GetObject());
		else
			_context->Push(NewContinuation(else_).GetObject());

		_break = true;
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
		Push(_continuation.GetObject());
		break;

	case Operation::Remove:
		Remove(_tree->GetRoot(), _continuation->GetScope(), Pop());
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
		ConstDeref<BasePointer<FunctionBase> >(Q)->Invoke(*Q.GetRegistry(), *_data);
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
		_data->Clear();
		break;

	case Operation::Depth:
		Push(New(_data->Size()));
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
		Push(_continuation->GetScope());
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
			_tree->SetScope(GetStorageBase(_tree->GetScope()).Get(ConstDeref<Label>(id)));
		else
			_tree->SetScope(ConstDeref<Pathname>(id));
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
		Object result = from.GetClass()->Minus(from, arg);
		from.GetClass()->Assign(from, result);
	}
		break;

	case Operation::MulEquals:
	{
		Object arg = Pop();
		Object from = Pop();
		Object result = from.GetClass()->Multiply(from, arg);
		from.GetClass()->Assign(from, result);
	}
		break;

	case Operation::DivEquals:
	{
		Object arg = Pop();
		Object from = Pop();
		Object result = from.GetClass()->Divide(from, arg);
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
		if (!_continuation->HasScope())
			_continuation->SetScope(New<void>().GetObject());

		Set(_tree->GetRoot(), _continuation->GetScope(), ident, value);
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

		method->Invoke(object, *_data);
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
