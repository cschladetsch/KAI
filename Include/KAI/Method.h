
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_METHOD_H
#	define KAI_METHOD_H

KAI_BEGIN

/// Hidden implementation for the Method<> type
namespace method_detail
{
	template <class Method>
	struct ConstMethodBase : MethodBase
	{
		typedef Method MethodType;
		Method method;
		ConstMethodBase(Method M, const Label &N) : method(M), MethodBase(Constness::Const, N) { }
		void NonConstInvoke(const Object & Q, Stack &S)
		{
			ConstInvoke(Q, S);
		}

	protected:
		ConstMethodBase(Method M, const Label &N, Constness C) : method(M), MethodBase(C, N) { }
	};

	template <class Method>
	struct MutatingMethodBase : ConstMethodBase<Method>
	{
		MutatingMethodBase(Method M, const Label &N) : ConstMethodBase<Method>(M, N, Constness::Mutable) { }
		void ConstInvoke(const Object &, Stack &)
		{
			KAI_THROW_1(ConstError, "Mutating method");
		}
	};

	template <class M, class T, class R, bool C, class A0 = meta::Null, class A1 = meta::Null, class A2 = meta::Null>
	struct SignedMethodBase : ConstMethodBase<M>
	{
		using MethodBase::return_type;
		using MethodBase::class_type;
		using MethodBase::constness;
		using MethodBase::arguments;

		SignedMethodBase(M N, const Label &O) : ConstMethodBase<M>(N, O)
		{
			return_type = Type::Traits<R>::Number;
			class_type = Type::Traits<T>::Number;
			AddArgument(Type::Traits<A0>::Number);
			AddArgument(Type::Traits<A1>::Number);
			AddArgument(Type::Traits<A2>::Number);
			constness = C ? Constness::Const : Constness::Mutable;
		}
		void AddArgument(Type::Number N)
		{
			if (N != Type::Traits<meta::Null>::Number)
				arguments.push_back(N);
		}
	};

	template <class M, class T, class R, class A0, class A1, class A2>
	struct SignedMethodBase<M,T,R,false,A0,A1,A2> : MutatingMethodBase<M>
	{
		using MethodBase::return_type;
		using MethodBase::class_type;
		using MethodBase::constness;
		using MethodBase::arguments;
		SignedMethodBase(M N, const Label &O) : MutatingMethodBase<M>(N, O)
		{
			return_type = Type::Traits<R>::Number;
			class_type = Type::Traits<T>::Number;
			AddArgument(Type::Traits<A0>::Number);
			AddArgument(Type::Traits<A1>::Number);
			AddArgument(Type::Traits<A2>::Number);
			constness = Constness::Mutable;
		}
		void AddArgument(Type::Number N)
		{
			if (N != Type::Traits<meta::Null>::Number)
				arguments.push_back(N);
		}
	};

	/// Store a method. this can be later invoked, by taking arguments from a stack
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
					struct WithArgs : SignedMethodBase<ReturnType (T::*)() const, T,ReturnType,true>
					{
						typedef SignedMethodBase<ReturnType (T::*)() const, T,ReturnType,true> Parent;
						WithArgs(typename Parent::MethodType M, const Label &N) : Parent(M,N) { }
						void ConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<const T> servant = Q;
							stack.Push(servant.New<ReturnType>(((*servant).*method)()));
						}
					};
				};
				template <>
				struct Returning<void>
				{
					template <class,class,class>
					struct WithArgs : SignedMethodBase<void (T::*)() const, T, void, true>
					{
						typedef ConstMethodBase<void (T::*)() const> Parent;
						WithArgs(MethodType M, const Label &N) : Parent(M, N) { }
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
					struct WithArgs : SignedMethodBase<ReturnType (T::*)(), T, ReturnType, false>
					{
						typedef SignedMethodBase<ReturnType (T::*)(), T, ReturnType, false> Parent;
						WithArgs(typename Parent::MethodType M, const Label &N) : Parent(M, N) { }
						void NonConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<T> servant = Q;
							stack.Push(servant.New<ReturnType>(((*servant).*method)()));
						}
					};
				};
				template <>
				struct Returning<void>
				{
					template <class,class,class>
					struct WithArgs : SignedMethodBase<void (T::*)(), T, void, false>
					{
						typedef SignedMethodBase<void (T::*)(), T, void, false> Parent;
						WithArgs(MethodType M, const Label &N) : Parent(M, N) { }
						void NonConstInvoke(const Object & Q, Stack &)
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
					struct WithArgs : SignedMethodBase<ReturnType (T::*)(A0) const, T, ReturnType, true, A0>
					{
						typedef SignedMethodBase<ReturnType (T::*)(A0) const, T, ReturnType, true, A0> Parent;
						WithArgs(typename Parent::MethodType M, const Label &N) : Parent(M, N) { }
						void ConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<const T> servant = Q;
							PointerType<A0>::Type a0 = stack.Pop();
							stack.Push(Q.New<ReturnType>(((*servant).*method)(Deref<A0>(a0))));
						}
					};
				};
				template <>
				struct Returning<void>
				{
					template <class A0,class,class>
					struct WithArgs : SignedMethodBase<void (T::*)(A0) const, T, void, true, A0>
					{
						typedef SignedMethodBase<void (T::*)(A0) const, T, void, true, A0> Parent;
						WithArgs(typename Parent::MethodType M, const Label &N) : Parent(M, N) { }
						void ConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<const T> servant = Q;
							PointerType<A0>::Type a0 = stack.Pop();
							((*servant).*method)(Deref<A0>(a0));
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
					struct WithArgs : SignedMethodBase<ReturnType (T::*)(A0), T, ReturnType, false, A0>
					{
						typedef SignedMethodBase<ReturnType (T::*)(A0), T, ReturnType, false, A0> Parent;
						WithArgs(typename Parent::MethodType M, const Label &N) : Parent(M, N) { }
						void NonConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<T> servant = Q;
							PointerType<A0>::Type a0 = stack.Pop();
							stack.Push(Q.New(((*servant).*method)(Deref<A0>(a0))));
						}
					};
				};
				template <>
				struct Returning<void>
				{
					template <class A0,class,class>
					struct WithArgs : SignedMethodBase<void (T::*)(A0), T, void, false, A0>
					{
						typedef SignedMethodBase<void (T::*)(A0), T, void, false, A0> Parent;
						WithArgs(typename Parent::MethodType M, const Label &N) : Parent(M, N) { }
						void NonConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<T> servant = Q;
							PointerType<A0>::Type a0 = stack.Pop();
							((*servant).*method)(Deref<A0>(a0));
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
					struct WithArgs : SignedMethodBase<ReturnType (T::*)(A0,A1) const, T, ReturnType, true, A0, A1>
					{
						typedef SignedMethodBase<ReturnType (T::*)(A0,A1) const, T, ReturnType, true, A0, A1> Parent;
						WithArgs(typename Parent::MethodType M, const Label &N) : Parent(M, N) { }
						void ConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<const T> servant = Q;
							PointerType<A1>::Type a1 = stack.Pop();
							PointerType<A0>::Type a0 = stack.Pop();
							stack.Push(Q.New(((*servant).*method)(Deref<A0>(a0), Deref<A1>(a1))));
						}
					};
				};
				template <>
				struct Returning<void>
				{
					template <class A0,class A1,class>
					struct WithArgs : SignedMethodBase<void (T::*)(A0,A1) const, T, void, true, A0, A1>
					{
						typedef SignedMethodBase<void (T::*)(A0,A1) const, T, void, true, A0, A1> Parent;
						WithArgs(typename Parent::MethodType M, const Label &N) : Parent(M, N) { }
						void ConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<const T> servant = Q;
							PointerType<A1>::Type a1 = stack.Pop();
							PointerType<A0>::Type a0 = stack.Pop();
							((*servant).*method)(Deref<A0>(a0), Deref<A1>(a1));
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
					struct WithArgs : SignedMethodBase<ReturnType (T::*)(A0,A1), T, ReturnType, false, A0, A1>
					{
						typedef SignedMethodBase<ReturnType (T::*)(A0,A1), T, ReturnType, false, A0, A1> Parent;
						WithArgs(MethodType M, const Label &N) : Parent(M, N) { }
						void NonConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<T> servant = Q;
							PointerType<A1>::Type a1 = stack.Pop();
							PointerType<A0>::Type a0 = stack.Pop();
							stack.Push(Q.New(((*servant).*method)(Deref<A0>(a0), Deref<A1>(a1))));
						}
					};
				};
				template <>
				struct Returning<void>
				{
					template <class A0,class A1,class>
					struct WithArgs : SignedMethodBase<void (T::*)(A0,A1), T, void, false, A0, A1>
					{
						typedef SignedMethodBase<void (T::*)(A0,A1), T, void, false, A0, A1> Parent;
						WithArgs(MethodType M, const Label &N) : Parent(M, N) { }
						void NonConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<T> servant = Q;
							PointerType<A1>::Type a1 = stack.Pop();
							PointerType<A0>::Type a0 = stack.Pop();
							((*servant).*method)(Deref<A0>(a0), Deref<A1>(a1));
						}
					};
				};
			};
		};
		template <>
		struct WithArity<3>
		{
			template <bool>
			struct Const
			{
				template <class ReturnType>
				struct Returning
				{
					template <class A0,class A1,class A2>
					struct WithArgs : SignedMethodBase<ReturnType (T::*)(A0,A1,A2) const, T, ReturnType, true, A0, A1, A2>
					{
						typedef SignedMethodBase<ReturnType (T::*)(A0,A1,A2) const, T, ReturnType, true, A0, A1, A2> Parent;
						WithArgs(typename Parent::MethodType M, const Label &N) : Parent(M, N) { }
						void ConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<const T> servant = Q;
							PointerType<A2>::Type a2 = stack.Pop();
							PointerType<A1>::Type a1 = stack.Pop();
							PointerType<A0>::Type a0 = stack.Pop();
							stack.Push(Q.New(((*servant).*method)(Deref<A0>(a0), Deref<A1>(a1), Deref<A2>(a2))));
						}
					};
				};
				template <>
				struct Returning<void>
				{
					template <class A0,class A1,class A2>
					struct WithArgs : SignedMethodBase<void (T::*)(A0,A1,A2) const, T, void, true, A0, A1, A2>
					{
						typedef SignedMethodBase<void (T::*)(A0,A1,A2) const, T, void, true, A0, A1, A2> Parent;
						WithArgs(typename Parent::MethodType M, const Label &N) : Parent(M, N) { }
						void ConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<const T> servant = Q;
							PointerType<A2>::Type a2 = stack.Pop();
							PointerType<A1>::Type a1 = stack.Pop();
							PointerType<A0>::Type a0 = stack.Pop();
							((*servant).*method)(Deref<A0>(a0), Deref<A1>(a1), Deref<A2>(a2));
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
					template <class A0,class A1,class A2>
					struct WithArgs : SignedMethodBase<ReturnType (T::*)(A0,A1,A2), T, ReturnType, false, A0, A1, A2>
					{
						typedef SignedMethodBase<ReturnType (T::*)(A0,A1,A2), T, ReturnType, false, A0, A1, A2> Parent;
						WithArgs(MethodType M, const Label &N) : Parent(M, N) { }
						void NonConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<T> servant = Q;
							PointerType<A2>::Type a2 = stack.Pop();
							PointerType<A1>::Type a1 = stack.Pop();
							PointerType<A0>::Type a0 = stack.Pop();
							stack.Push(Q.New(((*servant).*method)(Deref<A0>(a0), Deref<A1>(a1), Deref<A2>(a2))));
						}
					};
				};
				template <>
				struct Returning<void>
				{
					template <class A0,class A1,class A2>
					struct WithArgs : SignedMethodBase<void (T::*)(A0,A1,A2), T, void, false, A0, A1, A2>
					{
						typedef SignedMethodBase<void (T::*)(A0,A1,A2), T, void, false, A0, A1, A2> Parent;
						WithArgs(MethodType M, const Label &N) : Parent(M, N) { }
						void NonConstInvoke(const Object & Q, Stack &stack)
						{
							Pointer<T> servant = Q;
							PointerType<A2>::Type a2 = stack.Pop();
							PointerType<A1>::Type a1 = stack.Pop();
							PointerType<A0>::Type a0 = stack.Pop();
							((*servant).*method)(Deref<A0>(a0), Deref<A1>(a1), Deref<A2>(a2));
						}
					};
				};
			};
		};
	};
	/// Helper structure for the compiler. The following declaration fails for VS2005 (but works in VS2008):
	///
	/// Method<T>::WithArity<meta::Arity<A0,A1,A2>::Value>::Const<C>::Returning<R>::WithArgs<A0,A1,A2>
	///
	/// This structure exists merely to spell out to the compiler what I'm doing. It is otherwise unnecessary.
	template <class T, class R, bool C, class A0 = meta::Null, class A1 = meta::Null, class A2 = meta::Null>
	struct HelpCompiler
	{
		enum { Arity = meta::Arity<A0,A1,A2>::Value };
		typedef Method<T> Stage0;
		typedef typename Stage0::WithArity<Arity> Stage1;
		typedef typename Stage1::template Const<C> Stage2;
		typedef typename Stage2::template Returning<R> Stage3;
		typedef typename Stage3::template WithArgs<A0,A1,A2> Type;
	};
}

template <class T, class R, bool C, class A0 = meta::Null, class A1 = meta::Null, class A2 = meta::Null>
struct Method : method_detail::HelpCompiler<T,R,C,A0,A1,A2>::Type
{
	typedef typename method_detail::HelpCompiler<T,R,C,A0,A1,A2>::Type Parent;
	Method(typename Parent::MethodType M, const Label &L) : Parent(M, L) { }
};

template <class R, class T>
MethodBase *MakeMethod(R (T::*method)(), const Label &N)
{
	return new Method<T,R,false>(method, N);
}

template <class R, class T>
MethodBase *MakeMethod(R (T::*method)() const, const Label &N)
{
	return new Method<T,R,true>(method, N);
}

template <class T, class R, class A0>
MethodBase *MakeMethod(R (T::*method)(A0), const Label &N)
{
	return new Method<T,R,false,A0>(method, N);
}

template <class T, class R, class A0>
MethodBase *MakeMethod(R (T::*method)(A0) const, const Label &N)
{
	return new Method<T,R,true,A0>(method, N);
}

template <class T, class R, class A0, class A1>
MethodBase *MakeMethod(R (T::*method)(A0, A1), const Label &N)
{
	return new Method<T,R,false,A0,A1>(method, N);
}

template <class T, class R, class A0, class A1>
MethodBase *MakeMethod(R (T::*method)(A0, A1) const, const Label &N)
{
	return new Method<T,R,true,A0,A1>(method, N);
}

template <class T, class R, class A0, class A1, class A2>
MethodBase *MakeMethod(R (T::*method)(A0, A1, A2), const Label &N)
{
	return new Method<T,R,false,A0,A1,A2>(method, N);
}

template <class T, class R, class A0, class A1, class A2>
MethodBase *MakeMethod(R (T::*method)(A0, A1, A2) const, const Label &N)
{
	return new Method<T,R,true,A0,A1,A2>(method, N);
}

KAI_END

#endif // KAI_METHOD_H

//EOF
