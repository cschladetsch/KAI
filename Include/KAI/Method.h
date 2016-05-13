#pragma once

#include "MethodBase.h"
#include "Detail/CallableBase.h"

KAI_BEGIN

namespace method_detail
{
	using namespace std;
	using namespace meta;
	using namespace detail;

	// a method that returns something and is const
	template <class T, class R, class... Args>
	struct MethodConst : ConstMethodBase<R (T::*)(Args...) const>
	{
		typedef R (T::*MethodType)(Args...) const;
		typedef ConstMethodBase<MethodType> Parent;
		MethodType meth;
		tuple<Args...> _args;
		static int constexpr arity = (int)sizeof...(Args);

		MethodConst(MethodType m, const Label &N) : meth(m), Parent(m, N) { }

		void ConstInvoke(const Object &servant, Stack &stack)
		{
			detail::Add<arity - 1>::Arg(stack, _args);
			stack.Push(servant.New(CallMethod(ConstDeref<T>(servant), meth, _args)));
		}
	};

	// a method that returns void and is const
	template <class T, class... Args>
	struct VoidMethodConst : ConstMethodBase<void (T::*)(Args...) const>
	{
		typedef void (T::*MethodType)(Args...) const;
		typedef MutatingMethodBase<MethodType> Parent;
		static size_t constexpr arity = sizeof...(Args);
		MethodType meth;
		tuple<Args...> _args;
		VoidMethodConst(MethodBase m, const Label &N) 
			: meth(m), Parent(m, N) { }

		void ConstInvoke(const Object& servant, Stack &stack)
		{
			detail::Add<arity - 1>::Arg(stack, _args);
			CallMethod(ConstDeref<T>(servant), meth, _args);
		}
	};

	// a method that returns void  and is not const
	template <class T, class... Args>
	struct VoidMethod : MutatingMethodBase<void (T::*)(Args...)>
	{
		typedef void (T::*MethodType)(Args...);
		typedef MutatingMethodBase<MethodType> Parent;
		static size_t constexpr arity = sizeof...(Args);
		MethodType meth;
		tuple<Args...> _args;

		VoidMethod(MethodType m, const Label &N) : meth(m), Parent(m, N) { }

		void Invoke(Object &servant, Stack &stack)
		{
			detail::Add<arity - 1>::Arg(stack, _args);
			CallMethod(Deref<T>(servant), meth, _args);
		}
	};

	// a method that returns something and is not const
	template <class T, class R, class... Args>
	struct Method : MutatingMethodBase<R (T::*)(Args...)>
	{
		typedef R (T::*MethodType)(Args...);
		typedef MutatingMethodBase<MethodType> Parent;
		static size_t constexpr arity = sizeof...(Args);
		MethodType meth;
		tuple<Args...> _args;

		Method(MethodType m, const Label &N) : meth(m), Parent(m, N) { }

		void Invoke(Object &servant, Stack &stack)
		{
			detail::Add<arity - 1>::Arg(stack, _args);
			stack.Push(servant.New(Deref<T>(servant)(CallMethod(servant, meth, _args))));
		}
	};

	template  <bool V, bool C, class T00, class T01, class T10, class T11>
	struct Select
	{
		typedef typename If<V,
				typename If<C, T11, T01>::Type,
				typename If<C, T01, T00>::Type>::Type Type;
	};

	/// selects implementation based on whether the target method
	/// returns void or not
	template <class T, class R, bool C, class... Args>
	struct Selector
	{
		enum { VoidRet = SameType<R, void>::value };
		enum { Const = C };

		typedef typename Select<VoidRet, Const
				 , Method<T, R, Args...> 	
				 , MethodConst<T, R, Args...>
				 , VoidMethod<T, Args...>	  		
				 , VoidMethodConst<T, Args...>	
				 >::Type

				 Type;
	};

} // namespace method_detail

template <class T, class R, bool C, class... Args>
struct Method : method_detail::Selector<T,R,C,Args...>::Type
{
	typedef typename method_detail::Selector<T,R,C,Args...>::Type 
		Parent;
	Method(typename Parent::MethodType M, const Label &L) 
		: Parent(M, L) { }
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

