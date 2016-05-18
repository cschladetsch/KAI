#if 0

#include "KAI/Test/Base.h"
#include "KAI/AccessorBase.h"
#include "KAI/Accessor.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestProperties.h"

KAI_BEGIN

struct Foobar2
{
	int N;
	String S;
	Pointer<int> P;

	Foobar2() : N(9) { }
};

struct Foobar3 : Foobar2
{
	Pointer<const String> Q;
	Object K;
};

KAI_TYPE_TRAITS(Foobar2, 700, Properties::None);
KAI_TYPE_TRAITS(Foobar3, 701, Properties::None);

KAI_END

KAI_TESTS_BEGIN

void TestProperties::Pre()
{
}

void TestProperties::Post()
{
}

void TestProperties::PrepareRegistry(Registry &R)
{
	R.AddClass<void>();
	R.AddClass<int>();
	R.AddClass<String>();
	R.AddClass<BasePointer<PropertyBase> >();
	ClassBuilder<Foobar2>(R, "Foobar2")
		.Methods
		.Properties
		("N", &Foobar2::N)
		("P", &Foobar2::P)
		;
	ClassBuilder<Foobar3>(R, "Foobar3")
		.Methods
		.Properties
		("N", &Foobar3::N)
		("P", &Foobar3::P)
		("Q", &Foobar3::Q)
		("K", &Foobar3::K)
		;
}

void TestProperties::TestConstruction()
{
	Registry R;
	PrepareRegistry(R);

	Pointer<Foobar2> foo = R.New<Foobar2>();

	property_detail::MakeAccessor<Foobar2, Foobar3, int> A2(&Foobar2::N, "N", CreateParams::Default);
	property_detail::MakeAccessor<Foobar2, Foobar2, int> A(&Foobar2::N, "N", CreateParams::Default);

	int N = Deref<int>(A.GetValue(foo));
	KAI_TEST_EQUIV(N, 9);

	AccessorBase *Nacc = (AccessorBase *)MakeProperty<Foobar2>(&Foobar2::N, "N", CreateParams::Default);
	int N2 = ConstDeref<int>(Nacc->GetValue(foo));
	KAI_TEST_EQUIV(N2, 9);

	ClassBase const *kfoo = R.GetClass<Foobar2>();
	ClassBase::Properties const &props = kfoo->GetProperties();
	KAI_TEST_FALSE(props.empty());
	KAI_TEST_FALSE(props.find("N") == props.end());

	PropertyBase const &prop = kfoo->GetProperty("N");
	Object M = prop.GetValue(foo);
	KAI_TEST_EQUIV(ConstDeref<int>(M), 9);
}

void TestProperties::TestBuiltins()
{
	Registry R;
	PrepareRegistry(R);
	Pointer<Foobar2> foo2 = R.New<Foobar2>();
	foo2->N = 42;
	SetProperty(foo2, "N", 128);
	KAI_TEST_EQUIV(foo2->N, 128);

	int N = GetProperty<int>(foo2, "N");
	KAI_TEST_EQUIV(N, 128);
}

void TestProperties::TestInheretance()
{
	Registry R;
	PrepareRegistry(R);
	Pointer<Foobar3> foo3 = R.New<Foobar3>();
	
	foo3.SetProperty("N", 321);
	KAI_TEST_EQUIV(foo3->N, 321);

	foo3->P = R.New(456);
	KAI_TEST_EQUIV(*foo3->P, 456);

	foo3.SetProperty("P", 987);
	KAI_TEST_EQUIV(*foo3->P, 987);

	foo3.SetProperty("P", R.New(789));
	KAI_TEST_EQUIV(*foo3->P, 789);

	int N = foo3.GetProperty<int>("N");
	KAI_TEST_EQUIV(N, 321);

	foo3.SetPropertyObject("Q", R.New<String>("hello"));
	KAI_TEST_EQUIV(foo3.GetProperty<String>("Q"), "hello");
	bool caught = false;
	try
	{
		foo3.SetProperty<String>("Q", "world");
	}
	catch (Exception::ConstError &)
	{
		caught = true;
	}
	KAI_TEST_TRUE(caught);
}

KAI_TESTS_END

#endif // KAI_UNIT_TESTS

//EOF
#endif
