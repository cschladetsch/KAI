#include "TestBase.h"

using namespace kai;
using namespace std;

namespace
{
	static bool called[8];

	void VF0()
	{
		called[0] = true;
	}

	void VF1(int)
	{
		called[1] = true;
	}

	void VF2(int, int)
	{
		called[2] = true;
	}

	void VF3(int, int, int)
	{
		called[3] = true;
	}

	int F0()
	{
		called[4] = true;
		return 0;
	}

	int F1(int)
	{
		called[5] = true;
		return 1;
	}

	int F2(int, int)
	{
		called[6] = true;
		return 2;
	}

	int F3(int, int, int)
	{
		called[7] = true;
		return 3;
	}

	void G0(Pointer<int>)
	{
	}

	/*Pointer<String> */Object G1(Pointer<int>)
	{
		return Object();
	}
}

struct FunctionTest : public ::testing::Test
{

protected:
	virtual void SetUp()
	{
		_reg.AddClass<void>("void");
		_reg.AddClass<int>("int");
		_reg.AddClass<String>("String");
		_reg.AddClass<Stack>("stack");
		_reg.AddClass<BasePointer<FunctionBase> >("Function");
		auto n = _reg.New<int>();
		_stack = _reg.New<Stack>();
		std::fill(called, called + sizeof(called) / sizeof(called[0]), false);
	}

	virtual void TearDown() 
	{
	}

	Registry _reg;
	Value<Stack> _stack;
};

TEST_F(FunctionTest, TestConstruction)
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

	EXPECT_EQ(vf0->GetReturnType(), Type::Traits<void>::Number);
	EXPECT_EQ(vf1->GetReturnType(), Type::Traits<void>::Number);
	EXPECT_EQ(vf2->GetReturnType(), Type::Traits<void>::Number);
	EXPECT_EQ(vf3->GetReturnType(), Type::Traits<void>::Number);

	EXPECT_EQ(vf0->GetArgumentTypes().size(), std::size_t(0));
	EXPECT_EQ(vf1->GetArgumentTypes().size(), std::size_t(1));
	EXPECT_EQ(vf2->GetArgumentTypes().size(), std::size_t(2));
	EXPECT_EQ(vf3->GetArgumentTypes().size(), std::size_t(3));

	EXPECT_EQ(vf1->GetArgumentTypes()[0], Type::Traits<int>::Number);
	EXPECT_EQ(vf1->GetArgumentType(0), Type::Traits<int>::Number);

	EXPECT_EQ(vf2->GetArgumentType(0), Type::Traits<int>::Number);
	EXPECT_EQ(vf2->GetArgumentType(1), Type::Traits<int>::Number);

	EXPECT_EQ(vf3->GetArgumentType(0), Type::Traits<int>::Number);
	EXPECT_EQ(vf3->GetArgumentType(1), Type::Traits<int>::Number);
	EXPECT_EQ(vf3->GetArgumentType(2), Type::Traits<int>::Number);

	vf0->Invoke(_reg, *_stack);
	ASSERT_TRUE(called[0]);

	_stack->Push(_reg.New(42));
	vf1->Invoke(_reg, *_stack);
	ASSERT_TRUE(called[1]);

	_stack->Push(_reg.New(5));
	_stack->Push(_reg.New(234));
	vf2->Invoke(_reg, *_stack);
	ASSERT_TRUE(called[2]);

	_stack->Push(_reg.New(5));
	_stack->Push(_reg.New(234));
	_stack->Push(_reg.New(123));
	vf3->Invoke(_reg, *_stack);
	ASSERT_TRUE(called[3]);

	//??	f0->Invoke(_reg, *_stack);
	//ASSERT_TRUE(called[4]);
	//EXPECT_EQ(ConstDeref<int>(_stack->Pop()), 0);

	_stack->Push(_reg.New(42));
	f1->Invoke(_reg, *_stack);
	ASSERT_TRUE(called[5]);
	EXPECT_EQ(ConstDeref<int>(_stack->Pop()), 1);

	_stack->Push(_reg.New(5));
	_stack->Push(_reg.New(234));
	f2->Invoke(_reg, *_stack);
	ASSERT_TRUE(called[6]);
	EXPECT_EQ(ConstDeref<int>(_stack->Pop()), 2);

	_stack->Push(_reg.New(5));
	_stack->Push(_reg.New(234));
	_stack->Push(_reg.New(123));
	f3->Invoke(_reg, *_stack);
	ASSERT_TRUE(called[7]);
	EXPECT_EQ(ConstDeref<int>(_stack->Pop()), 3);

	_stack->Push(_reg.New(456));
	//TODO g0->Invoke(_reg, *_stack);
}

// TODO: Test leaking of base pointers

//void FunctionTest::TestExecutor()
//{
//	// TODO
//	//registry.AddClass<Compiler>("Compiler");
//	//Value<Compiler> compiler = registry.New<Compiler>();
//}

