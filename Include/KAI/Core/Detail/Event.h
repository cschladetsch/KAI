#pragma once

#include <KAI/Core/Meta/Base.h>

// TODO: replace with variadic templates

/// Hide implementation detail to avoid polluting global scope

namespace event_detail
{
	using namespace meta;

	/// A Delegate in this context is something that can be invoked
	/// with a set of parameters. It is basically just a function pointer-
	/// but it could also be a method pointer or even another Event.
	/// The purpose of this is to abstract away what it is that will
	/// actually be called.
	/// This is very boring code; the repititon is to deal with arity.
	template <int>
	struct Delegate;

	struct HasVirtualDtor
	{
		virtual ~HasVirtualDtor() { }
	};

	template <>
	struct Delegate<0> 
	{
		template <class = Null, class = Null, class = Null>
		struct Given : HasVirtualDtor
		{
			virtual void Invoke() = 0;
		};
	};

	template <>
	struct Delegate<1>
	{
		template <class T0, class = Null, class = Null>
		struct Given : HasVirtualDtor
		{
			virtual void Invoke(T0) = 0;
		};
	};

	template <>
	struct Delegate<2>
	{
		template <class T0, class T1, class = Null>
		struct Given : HasVirtualDtor
		{
			virtual void Invoke(T0, T1) = 0;
		};
	};

	template <>
	struct Delegate<3>
	{
		template <class T0, class T1, class T2>
		struct Given : HasVirtualDtor
		{
			virtual void Invoke(T0, T1, T2) = 0;
		};
	};

	/// This serves to create a function sink. A function sink
	/// is a Delegate which stores a function pointer with the same signature.
	template <int N>
	struct CreateFunction;

	template <>
	struct CreateFunction<0>
	{ 
		template <class = Null, class = Null, class = Null>
		struct Given : Delegate<0>::Given<>
		{
			typedef void (*Function)();
			Function fun;
			Given(Function F) : fun(F) { }
			void Invoke()
			{
				fun();
			}
		};
	};

	template <>
	struct CreateFunction<1>
	{ 
		template <class T0, class = Null, class = Null>
		struct Given : Delegate<1>::Given<T0>
		{
			typedef void (*Function)(T0);
			Function fun;
			Given(Function F) : fun(F) { }
			void Invoke(T0 t0)
			{
				fun(t0);
			}
		};
	};

	template <>
	struct CreateFunction<2>
	{ 
		template <class T0, class T1, class = Null>
		struct Given : Delegate<2>::Given<T0,T1>
		{
			typedef void (*Function)(T0,T1);
			Function fun;
			Given(Function F) : fun(F) { }
			void Invoke(T0 t0, T1 t1)
			{
				fun(t0, t1);
			}
		};
	};

	template <>
	struct CreateFunction<3>
	{ 
		template <class T0, class T1, class T2>
		struct Given : Delegate<3>::Given<T0,T1,T2>
		{
			typedef void (*Function)(T0,T1,T2);
			Function fun;
			Given(Function F) : fun(F) { }
			void Invoke(T0 t0, T1 t1, T2 t2)
			{
				fun(t0, t1, t2);
			}
		};
	};

	/// This structres creates a method sink type. Just like with
	/// the CreateFunction<> structures, these structures
	/// create types that are Delegates. The difference is that
	/// these structures store a method, as well as a pointer
	/// to an object that will be used to invoke that method
	/// when the Delegate is invoked.
	template <int>
	struct CreateMethod;

	template <>
	struct CreateMethod<0>
	{
		template <class, class, class>
		struct Given
		{
			template <bool, class Class>
			struct Type : Delegate<0>::Given<>
			{
				typedef void (Class::*Method)();
				Method method;
				Class *object;
				Type(Class *Q, Method M) : object(Q), method(M) { }
				void Invoke()
				{
					(object->*method)();
				}
			};
			template <class Class>
			struct Type<true, Class> : Delegate<0>::Given<>
			{
				typedef void (Class::*Method)();
				Method method;
				Object object;
				Type(Object const &Q, Method M) : object(Q), method(M) { }
				void Invoke()
				{
					if (object.Exists())
						(Deref<Class>(object).*method)();
				}
			};
		};
	};

	template <>
	struct CreateMethod<1>
	{
		template <class T0, class = Null, class = Null>
		struct Given
		{
			template <bool, class Class>
			struct Type : Delegate<1>::Given<T0>
			{
				typedef void (Class::*Method)(T0);
				Method method;
				Class *object;
				Type(Class *Q, Method M) : object(Q), method(M) { }
				void Invoke(T0 t0)
				{
					(object->*method)(t0);
				}
			};
			template <class Class>
			struct Type<true, Class> : Delegate<1>::Given<T0>
			{
				typedef void (Class::*Method)(T0);
				Method method;
				Object object;
				Type(Object const &Q, Method M) : object(Q), method(M) { }
				void Invoke(T0 t0)
				{
					if (object.Exists())
						(Deref<Class>(object).*method)(t0);
				}
			};
		};
	};

	template <>
	struct CreateMethod<2>
	{
		template <class T0, class T1, class = Null>
		struct Given
		{
			template <bool, class Class>
			struct Type : Delegate<2>::Given<T0,T1>
			{
				typedef void (Class::*Method)(T0,T1);
				Method method;
				Class *object;
				Type(Class *Q, Method M) : object(Q), method(M) { }
				void Invoke(T0 t0, T1 t1)
				{
					(object->*method)(t0, t1);
				}
			};
			template <class Class>
			struct Type<true, Class> : Delegate<2>::Given<T0,T1>
			{
				typedef void (Class::*Method)(T0,T1);
				Method method;
				Object object;
				Type(Object const &Q, Method M) : object(Q), method(M) { }
				void Invoke(T0 t0, T1 t1)
				{
					if (object.Exists())
						(Deref<Class>(object).*method)(t0, t1);
				}
			};
		};
	};

	template <>
	struct CreateMethod<3>
	{
		template <class T0, class T1, class T2>
		struct Given
		{
			template <bool, class Class>
			struct Type : Delegate<3>::Given<T0,T1,T2>
			{
				typedef void (Class::*Method)(T0,T1,T2);
				Method method;
				Class *object;
				Type(Class *Q, Method M) : object(Q), method(M) { }
				void Invoke(T0 t0, T1 t1, T2 t2)
				{
					(object->*method)(t0, t1, t2);
				}
			};
			template <class Class>
			struct Type<true, Class> : Delegate<3>::Given<T0,T1,T2>
			{
				typedef void (Class::*Method)(T0,T1,T2);
				Method method;
				Object object;
				Type(Object const &Q, Method M) : object(Q), method(M) { }
				void Invoke(T0 t0, T1 t1, T2 t2)
				{
					if (object.Exists())
						(Deref<Class>(object).*method)(t0, t1, t2);
				}
			};
		};
	};

	/// Internal DelegateType wrapper
	struct DelegateType
	{
		enum Type { None, Function, Method, Event };
		Type type;
		DelegateType(Type T = None) : type(T) { }
		friend bool operator==(DelegateType A, DelegateType B) { return A.type == B.type; }
		friend bool operator!=(DelegateType A, DelegateType B) { return A.type != B.type; }
	}; 

	/// Invokers store a list of Delegates. When invoked, an invoker
	/// calls all stored Delegates, passing the original arguments.
	template <int>
	struct AddInvoker;

	template <int N>
	struct InvokerBase
	{
		template <class T0 = Null, class T1 = Null, class T2 = Null>
		struct SinksType
		{
			typedef Delegate<N> DelRoot;
			typedef typename DelRoot::template Given<T0, T1, T2> *Del;
			typedef std::pair<DelegateType, Del> item;
			typedef std::list < item> Type;
		};
	};

	template <>
	struct AddInvoker<0>
	{
		template <class, class, class>
		struct Given : InvokerBase<0>
		{
			typedef typename SinksType<>::Type Sinks;
			Sinks sinks;
			void operator()()
			{
				Sinks::const_iterator A = sinks.begin(), B = sinks.end();
				for (; A != B; ++A)
				{
					// TODO: remove from sinks list if invocation fails
					A->second->Invoke();
				}
			}
		};
	};

	template <>
	struct AddInvoker<1> : InvokerBase<1>
	{
		template <class T0, class, class>
		struct Given
		{
			typedef typename SinksType<T0>::Type Sinks;
			Sinks sinks;
			void operator()(T0 t0)
			{
				typename Sinks::const_iterator A = sinks.begin(), B = sinks.end();
				for (; A != B; ++A)
					A->second->Invoke(t0);
			}
		};
	};

	template <>
	struct AddInvoker<2> : InvokerBase<2>
	{
		template <class T0, class T1, class>
		struct Given
		{
			typedef typename SinksType<T0,T1>::Type Sinks;
			Sinks sinks;
			void operator()(T0 t0, T1 t1)
			{
				typename Sinks::const_iterator A = sinks.begin(), B = sinks.end();
				for (; A != B; ++A)
					A->second->Invoke(t0, t1);
			}
		};
	};

	template <>
	struct AddInvoker<3> : InvokerBase<3>
	{
		template <class T0, class T1, class T2>
		struct Given
		{
			typedef typename SinksType<T0,T1,T2>::Type Sinks;
			Sinks sinks;
			void operator()(T0 t0, T1 t1, T2 t2)
			{
				typename Sinks::const_iterator A = sinks.begin(), B = sinks.end();
				for (; A != B; ++A)
					A->second->Invoke(t0, t1, t2);
			}
		};
	};

		template <class C, bool> struct MethodObjectBaseType { typedef C *Type; };
		template <class C> struct MethodObjectBaseType<C, true> { typedef Object Type; };

			template <class T>
			struct IsStorageType { enum { Value = false }; };
			template <>
			struct IsStorageType<StorageBase> { enum { Value = true }; };


	/// Helper structure to create an event type.
	/// This is specialised over the arity of the event signature
	template <int N>
	struct EventType
	{
		/// The main work to define the event type
		template <class T0, class T1, class T2>
		struct Create 
			: AddInvoker<N>::template Given<T0,T1,T2>	// add the correct operator() method to the type
		{
			~Create()
			{
				Clear();
			}

			typedef typename AddInvoker<N>::template Given<T0,T1,T2> Parent;

			typedef typename Parent::Sinks Sinks;
			
			/// Remove all delegates
			void Clear()
			{
				typename Sinks::const_iterator A = Parent::sinks.begin(), B = Parent::sinks.end();
				for (; A != B; ++A)
					delete A->second;
				Parent::sinks.clear();	
			}
			
			/// Create the function delegate type
			typedef typename CreateFunction<N>::template Given<T0,T1,T2> FunctionDelegate;	
			typedef typename FunctionDelegate::Function Function;

			/// Create the method delegate type. This is a structure as the method also needs a 
			/// class type whereas a function does not
			template <class C, bool system_type = false>
			struct MethodObject : CreateMethod<N>::template Given<T0,T1,T2>::template Type<system_type,C>
			{
				typedef typename CreateMethod<N>::template Given<T0,T1,T2>::template Type<system_type,C> Parent;
				typedef typename Parent::Method Method;

				MethodObject(typename MethodObjectBaseType<C, system_type>::Type Q, Method M) : Parent(Q, M) { }
			};

			/// Add a new function delegate
			void Add(Function fun)
			{
				Parent::sinks.push_back(std::make_pair(DelegateType::Function, new FunctionDelegate(fun)));
			}

			/// Remove an existing function delegate
			void Remove(Function fun)
			{
				typename Sinks::iterator A = Parent::sinks.begin(), B = Parent::sinks.end();
				for (; A != B; ++A)
				{
					if (A->first != DelegateType::Function)
						continue;
					FunctionDelegate *F = reinterpret_cast<FunctionDelegate *>(A->second);
					if (F->fun != fun)
						continue;
					delete A->second;
					Parent::sinks.erase(A);
					return;
				}			
			}

			// Helper function to create a new method delegate
			//template <class B, class C>
			//static MethodObject<C> Method(B *Q, typename MethodObject<B,C>::Method M)
			//{
			//	return MethodObject<B,C>(Q,M);
			//}

			template <class C>
			void AddMethod(C *base, typename MethodObject<C>::Method method)
			{
				Parent::sinks.push_back(std::make_pair(DelegateType::Method, new MethodObject<C>(base, method)));
			}
			template <class C>
			void AddMethod(Object object, typename MethodObject<C>::Method method)
			{
				Parent::sinks.push_back(std::make_pair(DelegateType::Method, new MethodObject<C,true>(object, method)));
			}
			template <class C>
			void AddMethod(std::pair<C *, typename MethodObject<C>::Method> bound)
			{
				Parent::sinks.push_back(std::make_pair(DelegateType::Method, new MethodObject<C>(bound.first, bound.second)));
			}

			//template <class C>
			//void AddMethod2(StorageBase *base, typename MethodObject<C>::Method method)
			//{
			//	//sinks.push_back(std::make_pair(DelegateType::Method, new MethodObject<C>(base, method)));
			//}

			///// Chain to another event
			//template <class U0>//, class U1, class U2>
			//void operator+=(typename Delegate<1>::template Given<U0> &chained)
			//{
			//	//*this += std::make_pair(&chained, &MethodObject<EventType<1>::Create<U0> >::operator());
			//}

			/// Remove an existing delegate
			template <class C, bool S>
			void Remove(MethodObject<C,S> bound)
			{
				typename Sinks::iterator A = Parent::sinks.begin(), B = Parent::sinks.end();
				for (; A != B; ++A)
				{
					if (A->first != DelegateType::Method)
						continue;
					MethodObject<C,S> *M = reinterpret_cast<MethodObject<C,S> *>(A->second);
					if (M->object != bound.object || M->method != bound.method)
						continue;
					delete A->second;
					Parent::sinks.erase(A);
					return;
				}
			}

			template <class C>
			void Remove(Object object, typename MethodObject<C>::Method method)
			{
				Remove(MethodObject<C,true>(object, method));
			}
			template <class C>
			void Remove(C *object, typename MethodObject<C>::Method method)
			{
				Remove(MethodObject<C>(object, method));
			}
			template <class C>
			void Remove(std::pair<C *, typename MethodObject<C>::Method> bound)
			{
				Remove(MethodObject<C>(bound.first, bound.second));
			}
			template <class C>
			void Remove(std::pair<Object, typename MethodObject<C>::Method> bound)
			{
				Remove(MethodObject<C,true>(bound.first, bound.second));
			}
		};
	};
} // end namespace event_detail

//EOF
