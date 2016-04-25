
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_FUNCTION_H
#	define KAI_FUNCTION_H

KAI_BEGIN

/// Do not pollute current namespace
namespace function_detail
{
	template <class R, class A0 = meta::Null, class A1 = meta::Null, class A2 = meta::Null, class A3 = meta::Null>
	struct SignedFunction : FunctionBase
	{
		SignedFunction(const Label &L) : FunctionBase(L)
		{
			return_type = Type::Traits<R>::Number;
			AddArgument(Type::Traits<A0>::Number);
			AddArgument(Type::Traits<A1>::Number);
			AddArgument(Type::Traits<A2>::Number);
			AddArgument(Type::Traits<A3>::Number);
		}
		void AddArgument(Type::NumberEnum N)
		{
			if (N != Type::Traits<meta::Null>::Number)
				arguments.push_back(N);
		}
	};

	/// this could be reduced further using the PP at the cost of maintainability(!)
	struct Function
	{
		template <int>
		struct WithArity;

		template <>
		struct WithArity<0>
		{
			template <class ReturnType>
			struct Returning
			{
				template <class,class,class,class>
				struct WithArgs : SignedFunction<ReturnType>
				{
					typedef ReturnType (*FunctionType)();
					FunctionType function;
					WithArgs(FunctionType F, const Label &L) : function(F), SignedFunction<ReturnType>(L) { }
					void Invoke(Registry &R, Stack &stack)
					{
						stack.Push(R.New(function()));
					}
				};
			};
			template <>
			struct Returning<void>
			{
				template <class,class,class,class>
				struct WithArgs : SignedFunction<void>
				{
					typedef void (*FunctionType)();
					FunctionType function;
					WithArgs(FunctionType F, const Label &L) : function(F), SignedFunction<void>(L) { }
					void Invoke(Registry &, Stack &)
					{
						function();
					}
				};
			};
		};

		template <>
		struct WithArity<1>
		{
			template <class ReturnType>
			struct Returning
			{
				template <class A0,class,class,class>
				struct WithArgs : SignedFunction<ReturnType, A0>
				{
					typedef ReturnType (*FunctionType)(A0);
					FunctionType function;
					WithArgs(FunctionType F, const Label &L) : function(F),SignedFunction<ReturnType, A0>(L) { }
					void Invoke(Registry &R, Stack &stack)
					{
						PointerType<A0>::Type a0 = stack.Pop();
						stack.Push(R.New(function(ArgType<A0>::From(a0))));
					}
				};
			};
			template <>
			struct Returning<void>
			{
				template <class A0,class,class,class>
				struct WithArgs : SignedFunction<void, A0>
				{
					typedef void (*FunctionType)(A0); 
					FunctionType function;
					WithArgs(FunctionType F, const Label &L) : function(F), SignedFunction<void, A0>(L) { }
					void Invoke(Registry &, Stack &stack)
					{
						PointerType<A0>::Type a0 = stack.Pop();
						function(ArgType<A0>::From(a0));
					}
				};
			};
		};

		template <>
		struct WithArity<2>
		{
			template <class ReturnType>
			struct Returning
			{
				template <class A0,class A1,class,class>
				struct WithArgs : SignedFunction<ReturnType, A0,A1>
				{
					typedef ReturnType (*FunctionType)(A0,A1);
					FunctionType function;
					WithArgs(FunctionType F, const Label &L) : function(F), SignedFunction<ReturnType, A0,A1>(L) { }
					void Invoke(Registry &R, Stack &stack)
					{
						PointerType<A1>::Type a1 = stack.Pop();
						PointerType<A0>::Type a0 = stack.Pop();
						stack.Push(R.New(function(ArgType<A0>::From(a0), ArgType<A1>::From(a1))));
					}
				};
			};
			template <>
			struct Returning<void>
			{
				template <class A0,class A1,class,class>
				struct WithArgs : SignedFunction<void, A0,A1>
				{
					typedef void (*FunctionType )(A0, A1);
					FunctionType function;
					WithArgs(FunctionType F, const Label &L) : function(F), SignedFunction<void, A0,A1>(L) { }
					void Invoke(Registry &, Stack &stack)
					{
						PointerType<A1>::Type a1 = stack.Pop();
						PointerType<A0>::Type a0 = stack.Pop();
						function(ArgType<A0>::From(a0), ArgType<A1>::From(a1));
					}
				};
			};
		};
		template <>
		struct WithArity<3>
		{
			template <class ReturnType>
			struct Returning
			{
				template <class A0,class A1,class A2,class>
				struct WithArgs : SignedFunction<ReturnType, A0,A1,A2>
				{
					typedef ReturnType (*FunctionType)(A0,A1,A2);
					FunctionType function;
					WithArgs(FunctionType F, const Label &L) : function(F), SignedFunction<ReturnType, A0,A1,A2>(L) { }
					void Invoke(Registry &R, Stack &stack)
					{
						PointerType<A2>::Type a2 = stack.Pop();
						PointerType<A1>::Type a1 = stack.Pop();
						PointerType<A0>::Type a0 = stack.Pop();
						stack.Push(R.New(function(ArgType<A0>::From(a0), ArgType<A1>::From(a1), ArgType<A2>::From(a2))));
					}
				};
			};
			template <>
			struct Returning<void>
			{
				template <class A0,class A1,class A2,class>
				struct WithArgs : SignedFunction<void, A0,A1,A2>
				{
					typedef void (*FunctionType )(A0, A1,A2);
					FunctionType function;
					WithArgs(FunctionType F, const Label &L) : function(F), SignedFunction<void, A0,A1,A2>(L) { }
					void Invoke(Registry &, Stack &stack)
					{
						PointerType<A2>::Type a2 = stack.Pop();
						PointerType<A1>::Type a1 = stack.Pop();
						PointerType<A0>::Type a0 = stack.Pop();
						function(ArgType<A0>::From(a0), ArgType<A1>::From(a1), ArgType<A2>::From(a2));
					}
				};
			};
		};
		template <>
		struct WithArity<4>
		{
			template <class ReturnType>
			struct Returning
			{
				template <class A0,class A1,class A2,class A3>
				struct WithArgs : SignedFunction<ReturnType, A0,A1,A2,A3>
				{
					typedef ReturnType (*FunctionType)(A0,A1,A2,A3);
					FunctionType function;
					WithArgs(FunctionType F, const Label &L) : function(F), SignedFunction<ReturnType, A0,A1,A2,A3>(L) { }
					void Invoke(Registry &R, Stack &stack)
					{
						PointerType<A3>::Type a3 = stack.Pop();
						PointerType<A2>::Type a2 = stack.Pop();
						PointerType<A1>::Type a1 = stack.Pop();
						PointerType<A0>::Type a0 = stack.Pop();
						stack.Push(R.New(function(ArgType<A0>::From(a0), ArgType<A1>::From(a1), ArgType<A2>::From(a2), ArgType<A3>::From(a3))));
					}
				};
			};
			template <>
			struct Returning<void>
			{
				template <class A0,class A1,class A2,class A3>
				struct WithArgs : SignedFunction<void, A0,A1,A2,A3>
				{
					typedef void (*FunctionType )(A0, A1,A2,A3);
					FunctionType function;
					WithArgs(FunctionType F, const Label &L) : function(F), SignedFunction<void, A0,A1,A2,A3>(L) { }
					void Invoke(Registry &, Stack &stack)
					{
						PointerType<A3>::Type a3 = stack.Pop();
						PointerType<A2>::Type a2 = stack.Pop();
						PointerType<A1>::Type a1 = stack.Pop();
						PointerType<A0>::Type a0 = stack.Pop();
						function(ArgType<A0>::From(a0), ArgType<A1>::From(a1), ArgType<A2>::From(a2), ArgType<A3>::From(a3));
					}
				};
			};
		};
	};

	/// Helper structure for the compiler. The following declaration fails for VS2005 (but works in VS2008):
	///
	/// Function<T>::WithArity<meta::Arity<A0,A1,A2>::Value>::Returning<R>::WithArgs<A0,A1,A2>
	///
	/// This structure exists merely to spell out to the compiler what I'm doing. It is otherwise unnecessary.
	template <class R, class A0 = meta::Null, class A1 = meta::Null, class A2 = meta::Null, class A3 = meta::Null>
	struct HelpCompiler
	{
		enum { Arity = meta::Arity<A0,A1,A2,A3>::Value };
		typedef typename Function::WithArity<Arity> Stage0;
		typedef typename Stage0::template Returning<R> Stage1;
		typedef typename Stage1::template WithArgs<A0,A1,A2,A3> Type;
	};
}

template <class R, class A0 = meta::Null, class A1 = meta::Null, class A2 = meta::Null, class A3 = meta::Null>
struct Function : function_detail::HelpCompiler<R,A0,A1,A2,A3>::Type
{
	typedef typename function_detail::HelpCompiler<R,A0,A1,A2,A3>::Type Parent;
	Function(typename Parent::FunctionType M, const Label &N) : Parent(M, N) { }
};

#define KAI_UNNAMED_FUNCTION_NAME "UnnamedFunction"

inline FunctionBase *AddDescription(FunctionBase *F, const char *D)
{
	if (D)
		F->Description = D;
	return F;
}

template <class R>
FunctionBase *MakeFunction(R (*func)(), const Label &L = KAI_UNNAMED_FUNCTION_NAME, const char *D = 0)
{
	return AddDescription(new Function<R>(func, L), D);
}

template <class R, class A0>
FunctionBase *MakeFunction(R (*func)(A0), const Label &L = KAI_UNNAMED_FUNCTION_NAME, const char *D = 0)
{
	return AddDescription(new Function<R,A0>(func, L), D);
}

template <class R, class A0, class A1>
FunctionBase *MakeFunction(R (*func)(A0, A1), const Label &L = KAI_UNNAMED_FUNCTION_NAME, const char *D = 0)
{
	return AddDescription(new Function<R,A0,A1>(func, L), D);
}

template <class R, class A0, class A1, class A2>
FunctionBase *MakeFunction(R (*func)(A0, A1, A2), const Label &L = KAI_UNNAMED_FUNCTION_NAME, const char *D = 0)
{
	return AddDescription(new Function<R,A0,A1,A2>(func, L), D);
}

template <class R, class A0, class A1, class A2, class A3>
FunctionBase *MakeFunction(R (*func)(A0, A1, A2,A3), const Label &L = KAI_UNNAMED_FUNCTION_NAME, const char *D = 0)
{
	return AddDescription(new Function<R,A0,A1,A2,A3>(func, L), D);
}

template <class Fun>
Object NewFunction(Object const &Q, Fun fun, const Label &L = KAI_UNNAMED_FUNCTION_NAME, const char *D = 0)
{
	return Q.New<BasePointer<FunctionBase> >(MakeFunction(fun, L, D));
}

template <class Function>
void AddFunction(Object const &Q, Function F, const char *N = KAI_UNNAMED_FUNCTION_NAME, const char *D = 0)
{
	Set(Q, Pathname(N), Q.New<BasePointer<FunctionBase> >(MakeFunction(F, N, D)));
}

KAI_END

#endif // KAI_FUNCTION_H

//EOF

