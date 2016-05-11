#pragma once

#include "Detail/CallableBase.h"

KAI_BEGIN

namespace method_detail
{
	using namespace std;
	using namespace meta;

	template <class Method>
	struct ConstMethodBase : MethodBase
	{
		typedef Method MethodType;
		Method method;
		ConstMethodBase(Method M, const Label &N) 
			: method(M), MethodBase(Constness::Const, N) { }
		void NonConstInvoke(const Object & Q, Stack &S)
		{
			ConstInvoke(Q, S);
		}

	protected:
		ConstMethodBase(Method M, const Label &N, Constness C) 
			: method(M), MethodBase(C, N) { }
	};

	template <class Method>
	struct MutatingMethodBase : ConstMethodBase<Method>
	{
	protected:

		MutatingMethodBase(Method M, const Label &N) 
			: ConstMethodBase<Method>(M, N, Constness::Mutable) { }

		void ConstInvoke(const Object &, Stack &)
		{
			KAI_THROW_1(ConstError, "Mutating method");
		}
	};

	template <class R, class T, bool C, class... Args>
	struct Method : MutatingMethodBase<R (T::*)(Args...)>
	{
		typedef R (T::*Meth)(Args...);
		typedef MutatingMethodBase<Meth> Parent;
		static size_t constexpr arity = sizeof...(Args);
		Meth meth;
		tuple<Args...> _args;

		Method(Meth m, const Label &N) : meth(m), Parent(N) { }

		void Invoke(Registry &reg, Stack &stack)
		{
			auto servant =  Deref<T>(stack.Pop());
			detail::Add<arity - 1>::Arg(stack, _args);
			auto &result = reg.New(CallMethod(servant, meth, _args));
			stack.Push(reg.New(result));
		}
	};


	template <class T, bool C, class... Args>
	struct VoidMethod
	{
		typedef void (T::*Meth)(Args...);
		typedef MutatingMethodBase<Meth> Parent;
		static size_t constexpr arity = sizeof...(Args);
		Meth meth;
		tuple<Args...> _args;
		VoidMethod(Meth m, const Label &N) : meth(m), Parent(N) { }

		void Invoke(Registry &reg, Stack &stack)
		{
			auto servant =  Deref<T>(stack.Pop());
			detail::Add<arity - 1>::Arg(stack, _args);
			reg.New(CallMethod(servant, meth, _args));
		}
	};

	template <class T, class R, bool C, class... Args>
	struct Selector
	{
		enum { VoidRet = SameType<R, Void>::value };
		typedef typename If<VoidRet, 
				VoidMethod<T, C, Args...>, 
				Method<R, T, C, Args...> >::Type Type; 
	};

} // namespace method_detail

template <class T, class R, bool C, class... Args>
struct Method : method_detail::Selector<T,R,C,Args...>::Type
{
	typedef typename method_detail::Selector<T,R,C,Args...>::Type Parent;
	Method(typename Parent::MethodType M, const Label &L) : Parent(M, L) { }
};

template <class T, class R, class... Args>
MethodBase *MakeMethod(R (T::*method)(Args...), const Label &N)
{
	return new Method<T,R,false, Args...>(method, N);
}

template <class T, class R, class... Args>
MethodBase *MakeMethod(R(T::*method)(Args...) const, const Label &N)
{
	return new Method<T,R,true, Args...>(method, N);
}

KAI_END

