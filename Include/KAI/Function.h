#pragma once

#include "FunctionBase.h"
#include "Detail/CallableBase.h"

KAI_BEGIN

namespace function_detail
{
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
	struct Fun : FunctionBase
	{
		typedef Ret (*Func)(Args...);
		Func fun;
		tuple<Args...> _args;
		static constexpr int arity = sizeof...(Args);

		Fun(Func f, const Label &N) : fun(f), FunctionBase(N) { }

		void Invoke(Registry &reg, Stack &stack)
		{
			Add<arity - 1>::Arg(stack, _args);
			stack.Push(reg.New<Ret>(CallFun(fun, _args)));
		}
	};

	template <class Ret, class... Args>
	struct Func
	{
		enum { VoidRet = SameType<Ret, void>::value };
		typedef typename If<VoidRet
			, VoidFun<Args...>
			, Fun<Ret , Args...> >::type Type;
	};
} // function_detail

template <class R, class... Args>
struct Function : function_detail::Func<R,Args...>::Type
{
	typedef typename function_detail::Func<R,Args...>::Type Parent;
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
FunctionBase *MakeFunction(R (*func)(Args...), const Label &L = KAI_UNNAMED_FUNCTION_NAME, const char *D = 0)
{
	return AddDescription(new Function<R, Args...>(func, L), D);
}

KAI_END

