#include "KAI/Core/BuiltinTypes.h"
#include "KAI/Core/FunctionBase.h"
#include "KAI/Core/Object.h"
#include "KAI/Core/Tree.h"
#include "KAI/Executor/SignedContinuation.h"
#include "KAI/Executor/Compiler.h"

#include <iostream>

using namespace std;

KAI_BEGIN

StringStream &operator<<(StringStream &stream, const Object &object);
StringStream &operator>>(StringStream &stream, Object &object);

const ClassBase *GetClass(Object const &Q);

void Executor::Create()
{
	_data = New<Stack>();
	_context = New<Stack>();
	_break = false;
	_traceLevel = 0;
	_stepNumber = 0;
}

bool Executor::Destroy()
{
	return true;
}

void Executor::Push(Object const &Q)
{
	if (Q.GetTypeNumber() == Type::Number::Object)
		Push(*_data, ConstDeref<Object>(Q));
	else
		Push(*_data, Q);
}

void Executor::Push(const std::pair<Object, Object> &P)
{
	Push(New(Pair(P.first, P.second)).GetObject());
}

Object Executor::ResolvePop()
{
	return Resolve(Pop(*_data));
}

Object Executor::Pop()
{
	return Pop(*_data);
}

Value<Stack> Executor::GetDataStack()
{
	return _data;
}

Value<const Stack> Executor::GetContextStack() const
{
	return Value<const Stack>(_context.GetConstObject());
}

void Executor::SetContinuation(Value<Continuation> C)
{
	_continuation = C;
	if (_continuation.Exists())
	{
		_continuation->Enter(this);
		_continuation->InitialStackDepth = _data->Size();
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
	if (_traceLevel > 2)
		KAI_TRACE_2("Entering", _continuation);

	Object next;
	for (;;)
	{
		_break = false;
		if (_continuation->Next(next))
		{
			KAI_TRY
			{
				if (_traceLevel > 0)
					KAI_TRACE_1(_data);

				if (_traceLevel > 1)
					KAI_TRACE_2(_continuation, next.ToString());

				Eval(next);
			}
			catch (Exception::Base &E)
			{
				KAI_TRACE_1(E);
				_data->Push(Reg().New<String>(E.ToString()));
				throw;
			}
		}
		else
			_break = true;

		if (_break)
		{
			if (_traceLevel > 0)
				KAI_TRACE_1(_data);

			NextContinuation();
			if (!_continuation.Exists())
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
	if (_context->Empty())
	{
		_continuation = Object();
		return;
	}

	if (_context->Empty())
	{
		KAI_TRACE_ERROR() << "Context stack is empty";
		KAI_NOT_IMPLEMENTED();
	}

	SetContinuation(_context->Pop());
}

void Executor::Push(Stack& L, Object const &Q)
{
	L.Push(Q);
}

void Executor::Eval(Object const &Q)
{
	//Dump(Q);
	_stepNumber++;

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
			Label const &label = ConstDeref<Label>(Q);
			if (label.Quoted)
			{
				Push(Q);
				return;
			}

			auto found = TryResolve(label);
			if (found.Exists())
				Push(found);
			else
			{
				KAI_THROW_1(ObjectNotFound, label.ToString());
			}
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
	Object Q = _tree->GetRoot();
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

void Executor::ContinueOnly(Value<Continuation> C)
{
	_context->Push(Object());	// add an empty context to break. this forces exection to stop after C is finished
	Continue(C);
}

template <class D>
Value<Array> Executor::ForEach(D const &C, Object const &F)
{
	Value<Array> R = New<Array>();
	typename D::const_iterator A = C.Begin(), B = C.End();
	for (; A != B; ++A)
	{
		Push(*A);
		_context->Push(Object());
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
	return _data->Top();
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
	if (_continuation.Exists())
	{
		Object scope = _continuation->GetScope();
		if (scope.Exists() && scope.Has(label))
			return scope.Get(label);
	}

	// search in parent scopes
	Stack const &scopes = *_context;
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
	return _tree->Resolve(label);
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
		if (_continuation.Exists() && Exists(_continuation->GetScope(), path))
			return Get(_continuation->GetScope(), path);

		// search in parent scopes
		Stack const &scopes = *_context;
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
	Object Q = _tree->Resolve(path);
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
	_continuation = Object();
	_context->Clear();
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

void Executor::SetTraceLevel(int N)
{
	_traceLevel = N;
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
	if (_traceLevel > 0)
	{
		if (_traceLevel > 1)
		{
			std::cout << "Stack:\n";
			DumpStack(*_data);
		}

		if (_traceLevel > 2)
		{
			std::cout << "Context:\n";
			for (auto c : *_context)
			{
				StringStream str;
				str << c;
				cout << str.ToString().c_str() << endl;
			}
		}

		std::cout << "\n[" << _stepNumber << "]: Eval: @" << *_continuation->index << " " << Q.ToString().c_str() << "\n";//std::endl;
	}
}

std::string Executor::PrintStack() const
{
	int n = 0;
	StringStream str;
	//auto const &stack = _data->GetStack();
	//Stack::const_reverse_iterator A = stack.rbegin(), B = stack.rend();
	for (const auto &A : _data->GetStack())
		str << "[" << n++ << "]: " << A <<  "\n";

	return str.ToString().c_str();
}

const char *ToString(Language l)
{
	switch (l)
	{
	case Language::None: return "none";
	case Language::Pi: return "pi";
	case Language::Rho: return "rho";
	case Language::Tau: return "tau";
	}
}

#include "./Executor.Perform.cpp"

int Process::trace = 0;

KAI_END

//EOF
