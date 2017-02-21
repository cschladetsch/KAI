

#include "KAI/Test/Base.h"
#ifdef KAI_UNIT_TESTS
#	include "KAI/Test/Base.h"
#	include "KAI/Tests/TestEvents.h"

KAI_BEGIN

void function0()
{
}
void function1(int)
{
}
void function2(int, String)
{
}
void function3(int, String, float)
{
}

struct Foobar
{
	void method0()
	{
	}
	void method1(int)
	{
	}
	void method2(int, String)
	{
	}
	void method3(int, String, float)
	{
	}
};

struct Grok
{
	void method0()
	{
	};
};

KAI_TYPE_TRAITS_BASIC(Grok, 666);

KAI_END

KAI_TESTS_BEGIN

void TestEvents::TestConstruction()
{
	Event<> event0;
	Event<int> event1;
	Event<int, String> event2;
	Event<int, String, float> event3;

	Foobar foo;

	event0.AddMethod(&foo, &Foobar::method0);
	event1.AddMethod(&foo, &Foobar::method1);
	event2.AddMethod(&foo, &Foobar::method2);
	event3.AddMethod(&foo, &Foobar::method3);

	event0.Add(function0);
	event1.Add(function1);
	event2.Add(function2);
	event3.Add(function3);

	event0();
	event1(42);
	event2(123, "hello");
	event3(-2, "world", 3.14f);

	Registry reg;
	reg.AddClass<void>("void");
	reg.AddClass<Grok>("Grok");

	Pointer<Grok> grok = reg.New<Grok>();
	event0.AddMethod(&*grok, &Grok::method0);
	event0.AddMethod<Grok>(grok, &Grok::method0);
	event0();
	grok.Delete();
	reg.GarbageCollect();
	event0();
}

void TestEvents::Pre()
{
}

void TestEvents::Post()
{
}

KAI_TESTS_END

#endif // KAI_UNIT_TESTS

//EOF
