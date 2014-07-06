
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestClosure.h"

KAI_BEGIN

//KAI_TYPE_TRAITS(TestClosureTest, 666, Type::Properties::None);

KAI_END

KAI_TESTS_BEGIN

#if 0

void TestFun(int N, String S)
{
	KAI_TRACE_2(N,S);
}

template <class F>
struct Closure
{
};

struct ClosureBase
{
	virtual void operator()() = 0;
};

template <class T>
struct ArgStorage
{
	typedef T const &Type;
};

template <class T>
struct ArgStorage<T &>
{
	typedef T &Type;
};

template <class T>
struct ArgStorage<T &>
{
	typedef T &Type;
};


template <class A0, class A1>
struct Closure<void (*)(A0,A1)> : ClosureBase
{
	typedef void (*Function)(A0,A1);
	Function fun;
	ArgStorage<A0>::Type a0;
	ArgStorage<A1>::Type a1;
	Closure(Function F, ArgParam<A0>::Type _a0, ArgParam<A1>::Type _a1) : fun(F), a0(_a0), a1(_a1) { }
	void operator()()
	{
		fun(a0,a1);
	}
};

void TestClosure::Test()
{
	Closure<TestFun>::Type C0(TestFun, N,S);
	C0();
}

#endif

KAI_TESTS_END

#endif // KAI_UNIT_TESTS

//EOF
