#pragma once

#include "FunctionBase.h"
#include "Detail/CallableBase.h"
#include "Meta/Base.h"

KAI_BEGIN

//namespace function_detail
//{
	using namespace detail;

	template <class... Args>
	struct VoidFun : FunctionBase
	{
		typedef void (*Func)(Args...);
		Func fun;
		tuple<Args...> _args;
		static constexpr int arity = sizeof...(Args);

		VoidFun(Func f, const Label &N) : fun(f), FunctionBase(N) { }

		void Invoke(Registry &reg, Stack &stack)
		{
			Add<arity - 1>::Arg(stack, _args);
			CallFun(fun, _args);
		}
	};

	template <class Ret, class... Args>
	struct NonVoidFun : FunctionBase
	{
		typedef Ret (*Func)(Args...);
		Func fun;
		tuple<Args...> _args;
		static constexpr int arity = sizeof...(Args);

		NonVoidFun(Func f, const Label &N) : fun(f), FunctionBase(N) { }

		void Invoke(Registry &reg, Stack &stack)
		{
			Add<arity - 1>::Arg(stack, _args);
			stack.Push(reg.New<Ret>(CallFun(fun, _args)));
		}
	};

	template <class Ret, class... Args>
	struct FunctionSelect
	{
		typedef Ret (*Fun)(Args...);
		enum { VoidRet = SameType<Ret, void>::value };
		typedef typename If<VoidRet
			, VoidFun<Args...>
			, NonVoidFun<Ret , Args...> >::Type Type;
	};
//} // function_detail

template <class R, class... Args>
struct Function : FunctionSelect<R, Args...>::Type
{
	typedef typename FunctionSelect<R,Args...>::Type Parent;
	Function(typename Parent::Func M, const Label &N) : Parent(M, N) { }
};

#define KAI_UNNAMED_FUNCTION_NAME "UnnamedFunction"

inline FunctionBase *AddDescription(FunctionBase *F, const char *D)
{
	if (D)
		F->Description = D;
	return F;
}

template <class R, class... Args>
FunctionBase *MakeFunction(R (*Fun)(Args...), const Label &L = KAI_UNNAMED_FUNCTION_NAME, const char *D = 0)
{
	return AddDescription(new Function<R, Args...>(Fun, L), D);
}

/// Make a new function object, add it to the given object
template <class R, class... Args>
Pointer<BasePointer<FunctionBase> > AddFunction(Object &root, R(*Fun)(Args...), const Label &L = KAI_UNNAMED_FUNCTION_NAME, const char *D = 0)
{
	auto fun = MakeFunction(Fun, L, D);
	auto ptr = root.New<BasePointer<FunctionBase> >(fun);
	root.SetChild(L, ptr);
	return ptr;
}

KAI_END

