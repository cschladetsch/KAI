
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_METHOD_H
#	define KAI_METHOD_H

KAI_BEGIN

/// Do not pollute current namespace
namespace method_detail
{
	template <class Method>
	struct ConstMethodBase : MethodBase
	{
		typedef Method MethodType;
		Method method;
		ConstMethodBase(Method M) : method(M) { }
		void Invoke(const Object & Q, Stack &S)
		{
			ConstInvoke(Q, S);
		}
	};

	template <class Method>
	struct MutatingMethodBase : MethodBase
	{
		typedef Method MethodType;
		Method method;
		MutatingMethodBase(Method M) : method(M) { }
		void ConstInvoke(const Object &Q, Stack &S)
		{
			KAI_THROW_1(ConstError, "Mutating method");
		}
	};

	/// this could be reduced further using the PP at the cost of maintainability(!)
	template <class T>
	struct Method
	{
		template <int>
		struct WithArity;

		template <>
		struct WithArity<0>
		{
			template <bool>
			struct Const
			{
				template <class ReturnType>
				struct Returning
				{
					template <class,class,class>
					struct WithArgs : ConstMethodBase<ReturnType (T::*)() const>
					{
						typedef ConstMethodBase<ReturnType (T::*)() const> Parent;
						WithArgs(typename Parent::MethodType M) : Parent(M) { }
						void ConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<const T> servant = Q;
							Pointer<ReturnType> R = Q.GetRegistry()->New<ReturnType>();
							*R = ((*servant).*method)();
							stack.Push(R);
						}
					};
				};
				template <>
				struct Returning<void>
				{
					template <class,class,class>
					struct WithArgs : ConstMethodBase<void (T::*)() const>
					{
						typedef ConstMethodBase<void (T::*)() const> Parent;
						WithArgs(MethodType M) : Parent(M) { }
						void ConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<const T> servant = Q;
							((*servant).*method)();
						}
					};
				};
			};
			template <>
			struct Const<false>
			{
				template <class ReturnType>
				struct Returning
				{
					template <class,class,class>
					struct WithArgs : MutatingMethodBase<ReturnType (T::*)()>
					{
						typedef MutatingMethodBase<ReturnType (T::*)()> Parent;
						WithArgs(typename Parent::MethodType M) : Parent(M) { }
						void Invoke(const Object & Q, Stack &stack)
						{
							Pointer<T> servant = Q;
							Pointer<ReturnType> R = Q.GetRegistry().New<ReturnType>();
							*R = ((*servant).*method)();
							stack.Push(R);
						}
					};
				};
				template <>
				struct Returning<void>
				{
					template <class,class,class>
					struct WithArgs : MutatingMethodBase<void (T::*)()>
					{
						typedef MutatingMethodBase<void (T::*)()> Parent;
						WithArgs(MethodType M) : Parent(M) { }
						void Invoke(const Object & Q, Stack &stack)
						{
							Pointer<T> servant = Q;
							((*servant).*method)();
						}
					};
				};
			};
		};

		template <>
		struct WithArity<1>
		{
			template <bool>
			struct Const
			{
				template <class ReturnType>
				struct Returning
				{
					template <class A0,class,class>
					struct WithArgs : ConstMethodBase<ReturnType (T::*)(A0) const>
					{
						typedef ConstMethodBase<ReturnType (T::*)(A0) const> Parent;
						WithArgs(typename Parent::MethodType M) : Parent(M) { }
						void ConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<const T> servant = Q;
							Pointer<A0> a0 = stack.Pop();
							Pointer<ReturnType> R = Q.GetRegistry().New<ReturnType>();
							*R = ((*servant).*method)(*a0);
							stack.Push(R);
						}
					};
				};
				template <>
				struct Returning<void>
				{
					template <class A0,class,class>
					struct WithArgs : ConstMethodBase<void (T::*)(A0) const>
					{
						typedef ConstMethodBase<void (T::*)(A0) const> Parent;
						WithArgs(typename Parent::MethodType M) : Parent(M) { }
						void ConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<const T> servant = Q;
							Pointer<A0> a0 = stack.Pop();
							((*servant).*method)(*a0);
						}
					};
				};
			};
			template <>
			struct Const<false>
			{
				template <class ReturnType>
				struct Returning
				{
					template <class A0,class,class>
					struct WithArgs : MutatingMethodBase<ReturnType (T::*)(A0)>
					{
						typedef MutatingMethodBase<ReturnType (T::*)(A0)> Parent;
						WithArgs(typename Parent::MethodType M) : Parent(M) { }
						void Invoke(const Object & Q, Stack &stack)
						{
							Pointer<T> servant = Q;
							Pointer<A0> a0 = stack.Pop();
							Pointer<ReturnType> R = Q.GetRegistry().New<ReturnType>();
							*R = ((*servant).*method)(*a0);
							stack.Push(R);
						}
					};
				};
				template <>
				struct Returning<void>
				{
					template <class A0,class,class>
					struct WithArgs : MutatingMethodBase<void (T::*)(A0)>
					{
						typedef MutatingMethodBase<void (T::*)(A0)> Parent;
						WithArgs(typename Parent::MethodType M) : Parent(M) { }
						void Invoke(const Object & Q, Stack &stack)
						{
							Pointer<T> servant = Q;
							Pointer<A0> a0 = stack.Pop();
							((*servant).*method)(*a0);
						}
					};
				};
			};
		};

		template <>
		struct WithArity<2>
		{
			template <bool>
			struct Const
			{
				template <class ReturnType>
				struct Returning
				{
					template <class A0,class A1,class>
					struct WithArgs : ConstMethodBase<ReturnType (T::*)(A0,A1) const>
					{
						typedef ConstMethodBase<ReturnType (T::*)(A0,A1) const> Parent;
						WithArgs(typename Parent::MethodType M) : Parent(M) { }
						void ConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<const T> servant = Q;
							Pointer<A1> a1 = stack.Pop();
							Pointer<A0> a0 = stack.Pop();
							Pointer<ReturnType> R = Q.GetRegistry()->New<ReturnType>();
							*R = ((*servant).*method)(*a0, *a1);
							stack.Push(R);
						}
					};
				};
				template <>
				struct Returning<void>
				{
					template <class A0,class A1,class>
					struct WithArgs : ConstMethodBase<void (T::*)(A0,A1) const>
					{
						typedef ConstMethodBase<void (T::*)(A0,A1) const> Parent;
						WithArgs(typename Parent::MethodType M) : Parent(M) { }
						void ConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<const T> servant = Q;
							Pointer<A1> a1 = stack.Pop();
							Pointer<A0> a0 = stack.Pop();
							((*servant).*method)(*a0, *a1);
						}
					};
				};
			};
			template <>
			struct Const<false>
			{
				template <class ReturnType>
				struct Returning
				{
					template <class A0,class A1,class>
					struct WithArgs : MutatingMethodBase<ReturnType (T::*)(A0,A1)>
					{
						typedef MutatingMethodBase<ReturnType (T::*)(A0,A1)> Parent;
						WithArgs(MethodType M) : Parent(M) { }
						void Invoke(const Object & Q, Stack &stack)
						{
							Pointer<T> servant = Q;
							Pointer<A1> a1 = stack.Pop();
							Pointer<A0> a0 = stack.Pop();
							Pointer<ReturnType> R = Q.New<ReturnType>();
							*R = ((*servant).*method)(*a0, *a1);
							stack.Push(R);
						}
					};
				};
				template <>
				struct Returning<void>
				{
					template <class A0,class A1,class>
					struct WithArgs : MutatingMethodBase<void (T::*)(A0,A1)>
					{
						typedef MutatingMethodBase<void (T::*)(A0,A1)> Parent;
						WithArgs(MethodType M) : Parent(M) { }
						void Invoke(const Object & Q, Stack &stack)
						{
							Pointer<T> servant = Q;
							Pointer<A1> a1 = stack.Pop();
							Pointer<A0> a0 = stack.Pop();
							((*servant).*method)(*a0, *a1);
						}
					};
				};
			};
			template <bool C, class R, class A0, class A1, class A2>
			struct Hack : Const<C>::Returning<R>::WithArgs<A0,A1,A2>
			{
			};
		};
	};

}

template <class T, class R, bool C, class A0 = meta::Null, class A1 = meta::Null, class A2 = meta::Null>
struct Method : method_detail::Method<T>::WithArity<meta::Arity<A0,A1,A2>::Value>::Hack<C,R,A0,A1,A2>
{
	typedef typename method_detail::Method<T>::WithArity<meta::Arity<A0,A1,A2>::Value>::Hack<C,R,A0,A1,A2> Parent;
	Method(typename Parent::MethodType M) : Parent(M) { }
};

template <class R, class T>
MethodBase *MakeMethod(R (T::*method)())
{
	return new Method<T,R,false>(method);
}

template <class R, class T>
MethodBase *MakeMethod(R (T::*method)() const)
{
	return new Method<T,R,true>(method);
}

template <class T, class R, class A0>
MethodBase *MakeMethod(R (T::*method)(A0))
{
	return new Method<T,R,false,A0>(method);
}

template <class T, class R, class A0>
MethodBase *MakeMethod(R (T::*method)(A0) const)
{
	return new Method<T,R,true,A0>(method);
}

template <class T, class R, class A0, class A1>
MethodBase *MakeMethod(R (T::*method)(A0, A1))
{
	return new Method<T,R,false,A0,A1>(method);
}

template <class T, class R, class A0, class A1>
MethodBase *MakeMethod(R (T::*method)(A0, A1) const)
{
	return new Method<T,R,true,A0,A1>(method);
}

KAI_END

#endif // KAI_METHOD_H

//EOF
