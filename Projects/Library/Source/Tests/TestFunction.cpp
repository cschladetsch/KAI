
#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestFunction.h"
#	include <iostream>

KAI_TESTS_BEGIN

namespace 
{
	static bool called[8];

	void VF0()
	{
		std::cout << "VF0" << std::endl;
		called[0] = true;
	}

	void VF1(int)
	{
		called[1] = true;
	}

	void VF2(int , int )
	{
		called[2] = true;
	}

	void VF3(int , int , int )
	{
		called[3] = true;
	}

	int F0()
	{
		called[4] = true;
		return 0;
	}

	int F1(int )
	{
		called[5] = true;
		return 1;
	}

	int F2(int , int )
	{
		called[6] = true;
		return 2;
	}

	int F3(int , int , int)
	{
		called[7] = true;
		return 3;
	}

	void G0(Pointer<int> )
	{
	}

	/*Pointer<String> */Object G1(Pointer<int> )
	{
		return Object();
	}
}

void TestFunction::Pre()
{
	registry.AddClass<void>("void");
	registry.AddClass<int>("int");
	registry.AddClass<String>("String");
	registry.AddClass<Stack>("stack");
	registry.AddClass<BasePointer<FunctionBase> >("Function");
	stack = registry.New<Stack>();
	std::fill(called, called + sizeof(called)/sizeof(called[0]), false);
}

void TestFunction::Post()
{
	registry.Clear();
}

void TestFunction::TestConstruction()
{
	FunctionBase *vf0 = MakeFunction(VF0, "VF0");
	FunctionBase *vf1 = MakeFunction(VF1);
	FunctionBase *vf2 = MakeFunction(VF2);
	FunctionBase *vf3 = MakeFunction(VF3);
//??	FunctionBase *f0 = MakeFunction(F0);
	FunctionBase *f1 = MakeFunction(F1);
	FunctionBase *f2 = MakeFunction(F2);
	FunctionBase *f3 = MakeFunction(F3);
	
	FunctionBase *g0 = MakeFunction(G0);
//	FunctionBase *g1 = MakeFunction(G1);

	KAI_TEST_EQUIV(vf0->GetReturnType(), Type::Traits<void>::Number);
	KAI_TEST_EQUIV(vf1->GetReturnType(), Type::Traits<void>::Number);
	KAI_TEST_EQUIV(vf2->GetReturnType(), Type::Traits<void>::Number);
	KAI_TEST_EQUIV(vf3->GetReturnType(), Type::Traits<void>::Number);

	KAI_TEST_EQUIV(vf0->GetArgumentTypes().size(), std::size_t(0));
	KAI_TEST_EQUIV(vf1->GetArgumentTypes().size(), std::size_t(1));
	KAI_TEST_EQUIV(vf2->GetArgumentTypes().size(), std::size_t(2));
	KAI_TEST_EQUIV(vf3->GetArgumentTypes().size(), std::size_t(3));

	KAI_TEST_EQUIV(vf1->GetArgumentType(0), Type::Traits<int>::Number);

	KAI_TEST_EQUIV(vf2->GetArgumentType(0), Type::Traits<int>::Number);
	KAI_TEST_EQUIV(vf2->GetArgumentType(1), Type::Traits<int>::Number);

	KAI_TEST_EQUIV(vf3->GetArgumentType(0), Type::Traits<int>::Number);
	KAI_TEST_EQUIV(vf3->GetArgumentType(1), Type::Traits<int>::Number);
	KAI_TEST_EQUIV(vf3->GetArgumentType(2), Type::Traits<int>::Number);

	vf0->Invoke(registry, *stack);
	KAI_TEST_TRUE(called[0]);

	stack->Push(registry.New(42));
	vf1->Invoke(registry, *stack);
	KAI_TEST_TRUE(called[1]);

	stack->Push(registry.New(5));
	stack->Push(registry.New(234));
	vf2->Invoke(registry, *stack);
	KAI_TEST_TRUE(called[2]);

	stack->Push(registry.New(5));
	stack->Push(registry.New(234));
	stack->Push(registry.New(123));
	vf3->Invoke(registry, *stack);
	KAI_TEST_TRUE(called[3]);

//??	f0->Invoke(registry, *stack);
	//KAI_TEST_TRUE(called[4]);
	//KAI_TEST_EQUIV(ConstDeref<int>(stack->Pop()), 0);

	stack->Push(registry.New(42));
	f1->Invoke(registry, *stack);
	KAI_TEST_TRUE(called[5]);
	KAI_TEST_EQUIV(ConstDeref<int>(stack->Pop()), 1);

	stack->Push(registry.New(5));
	stack->Push(registry.New(234));
	f2->Invoke(registry, *stack);
	KAI_TEST_TRUE(called[6]);
	KAI_TEST_EQUIV(ConstDeref<int>(stack->Pop()), 2);

	stack->Push(registry.New(5));
	stack->Push(registry.New(234));
	stack->Push(registry.New(123));
	f3->Invoke(registry, *stack);
	KAI_TEST_TRUE(called[7]);
	KAI_TEST_EQUIV(ConstDeref<int>(stack->Pop()), 3);

	stack->Push(registry.New(456));
	g0->Invoke(registry, *stack);

}

// TODO: Test leaking of base pointers

void TestFunction::TestExecutor()
{
	// TODO
	//registry.AddClass<Compiler>("Compiler");
	//Value<Compiler> compiler = registry.New<Compiler>();
}

KAI_TESTS_END

#endif

//EOF
