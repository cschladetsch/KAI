



#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestMethod.h"
#	include <iostream>

KAI_BEGIN

/// Used to test the Method system
struct MethodTester
{
	mutable std::vector<bool> called;
	mutable int A, B, C;

	MethodTester()
	{
		called.resize(12, false);
	}

	void M0()
	{
		called[0] = true;
	}

	void M1(int A)
	{
		this->A = A;
		called[1] = true;
	}

	void M2(int A, int B)
	{
		this->A = A;
		this->B = B;
		called[2] = true;
	}

	void M3(int A, int B, int C)
	{
		this->A = A;
		this->B = B;
		this->C = C;
		called[3] = true;
	}

	int M4()
	{
		called[4] = true;
		return 4;
	}

	int M5(int A)
	{
		this->A = A;
		called[5] = true;
		return A*2;
	}

	int M6(int A, int B)
	{
		this->A = A;
		this->B = B;
		called[6] = true;
		return A + B;
	}

	int M7(int A, int B, int C)
	{
		called[7] = true;
		this->A = A;
		this->B = B;
		this->C = C;
		return A + B + C;
	}

	int M8() const
	{
		called[8] = true;
		return 8;
	}

	int M9(int A) const
	{
		this->A = A;
		called[9] = true;
		return A*2;
	}

	int M10(int A, int B)
	{
		this->A = A;
		this->B = B;
		called[10] = true;
		return A + B;
	}

	int M11(int A, int B, int C)
	{
		this->A = A;
		this->B = B;
		this->C = C;
		called[11] = true;
		return A + B + C;
	}
};

KAI_TYPE_TRAITS_BASIC(MethodTester, 666);

KAI_END

KAI_TESTS_BEGIN

void TestMethod::TestConstruction()
{
	// use a Registry to store types and instances
	Registry R;
	R.AddClass<void>("void");
	R.AddClass<int>("int");

	// add the test class to the system
	ClassBuilder<MethodTester>(R, "MethodTester")
		.Methods
		("M0", &MethodTester::M0)
		("M1", &MethodTester::M1)
		("M2", &MethodTester::M2)
		("M3", &MethodTester::M3)
		("M4", &MethodTester::M4)
		("M5", &MethodTester::M5)
		("M6", &MethodTester::M6)
		("M7", &MethodTester::M7)
		("M8", &MethodTester::M8)
		("M9", &MethodTester::M9)
		("M10", &MethodTester::M10)
		("M11", &MethodTester::M11)
		;

	KAI_TEST_TRUE(R.GetClass<MethodTester>() != 0);
	KAI_TEST_EQUIV(R.GetClass("MethodTester"), R.GetClass(666));

	// get all the methods
	MethodBase *m0 = R.GetClass<MethodTester>()->GetMethod("M0");
	MethodBase *m1 = R.GetClass<MethodTester>()->GetMethod("M1");
	MethodBase *m2 = R.GetClass<MethodTester>()->GetMethod("M2");
	MethodBase *m3 = R.GetClass<MethodTester>()->GetMethod("M3");
	MethodBase *m4 = R.GetClass<MethodTester>()->GetMethod("M4");
	MethodBase *m5 = R.GetClass<MethodTester>()->GetMethod("M5");
	MethodBase *m6 = R.GetClass<MethodTester>()->GetMethod("M6");
	MethodBase *m7 = R.GetClass<MethodTester>()->GetMethod("M7");
	MethodBase *m8 = R.GetClass<MethodTester>()->GetMethod("M8");
	MethodBase *m9 = R.GetClass<MethodTester>()->GetMethod("M9");
	MethodBase *m10 = R.GetClass<MethodTester>()->GetMethod("M10");
	MethodBase *m11 = R.GetClass<MethodTester>()->GetMethod("M11");

	KAI_TEST_TRUE(m0 != 0);
	KAI_TEST_TRUE(m1 != 0);
	KAI_TEST_TRUE(m2 != 0);
	KAI_TEST_TRUE(m3 != 0);
	KAI_TEST_TRUE(m4 != 0);
	KAI_TEST_TRUE(m5 != 0);
	KAI_TEST_TRUE(m6 != 0);
	KAI_TEST_TRUE(m7 != 0);
	KAI_TEST_TRUE(m8 != 0);
	KAI_TEST_TRUE(m9 != 0);
	KAI_TEST_TRUE(m10 != 0);
	KAI_TEST_TRUE(m11 != 0);

	// prepare the tests. we need a stack to store the arguments, and an object to use to call the methods
	R.AddClass<Stack>("Stack");
	Pointer<Stack> stackPtr = R.New<Stack>();
	Pointer<MethodTester> foo = R.New<MethodTester>();
	Stack &stack = *stackPtr;

	// test the methods that return void
	m0->Invoke(foo, stack);
	KAI_TEST_TRUE(foo->called[0]);

	stack.Push(R.New(1));
	m1->Invoke(foo, stack);
	KAI_TEST_TRUE(foo->called[1]);
	KAI_TEST_EQUIV(foo->A, 1);

	stack.Push(R.New(1));
	stack.Push(R.New(2));
	m2->Invoke(foo, stack);
	KAI_TEST_TRUE(foo->called[2]);
	KAI_TEST_TRUE(foo->A == 1);
	KAI_TEST_TRUE(foo->B == 2);

	stack.Push(R.New(1));
	stack.Push(R.New(2));
	stack.Push(R.New(3));
	m3->Invoke(foo, stack);
	KAI_TEST_TRUE(foo->called[3]);
	KAI_TEST_TRUE(foo->A == 1);
	KAI_TEST_TRUE(foo->B == 2);
	KAI_TEST_TRUE(foo->C == 3);

	// non-void returns
	m4->Invoke(foo, stack);
	KAI_TEST_TRUE(foo->called[4]);
	KAI_TEST_EQUIV(ConstDeref<int>(stack.Pop()), 4);

	stack.Push(R.New(5));
	m5->Invoke(foo, stack);
	KAI_TEST_TRUE(foo->called[5]);
	KAI_TEST_TRUE(foo->A == 5);
	KAI_TEST_EQUIV(ConstDeref<int>(stack.Pop()), 10);

	stack.Push(R.New(1));
	stack.Push(R.New(2));
	m6->Invoke(foo, stack);
	KAI_TEST_TRUE(foo->called[6]);
	KAI_TEST_TRUE(foo->A == 1);
	KAI_TEST_TRUE(foo->B == 2);
	KAI_TEST_EQUIV(ConstDeref<int>(stack.Pop()), 3);

	stack.Push(R.New(1));
	stack.Push(R.New(2));
	stack.Push(R.New(3));
	m7->Invoke(foo, stack);
	KAI_TEST_TRUE(foo->called[7]);
	KAI_TEST_TRUE(foo->A == 1);
	KAI_TEST_TRUE(foo->B == 2);
	KAI_TEST_TRUE(foo->C == 3);
	KAI_TEST_EQUIV(ConstDeref<int>(stack.Pop()), 6);

	// const non-void returns
	m8->Invoke(foo, stack);
	KAI_TEST_TRUE(foo->called[8]);
	KAI_TEST_EQUIV(ConstDeref<int>(stack.Pop()), 8);

	stack.Push(R.New(5));
	m9->Invoke(foo, stack);
	KAI_TEST_TRUE(foo->called[9]);
	KAI_TEST_TRUE(foo->A == 5);
	KAI_TEST_EQUIV(ConstDeref<int>(stack.Pop()), 10);

	stack.Push(R.New(1));
	stack.Push(R.New(2));
	m10->Invoke(foo, stack);
	KAI_TEST_TRUE(foo->called[10]);
	KAI_TEST_TRUE(foo->A == 1);
	KAI_TEST_TRUE(foo->B == 2);
	KAI_TEST_EQUIV(ConstDeref<int>(stack.Pop()), 3);

	stack.Push(R.New(1));
	stack.Push(R.New(2));
	stack.Push(R.New(3));
	m11->Invoke(foo, stack);
	KAI_TEST_TRUE(foo->called[11]);
	KAI_TEST_TRUE(foo->A == 1);
	KAI_TEST_TRUE(foo->B == 2);
	KAI_TEST_TRUE(foo->C == 3);
	KAI_TEST_EQUIV(ConstDeref<int>(stack.Pop()), 6);

	KAI_TEST_TRUE(stack.Empty());

}

KAI_TESTS_END

#endif // KAI_UNIT_TESTS

//EOF
