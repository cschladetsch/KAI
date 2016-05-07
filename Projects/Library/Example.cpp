//#include "KAI/KAI.h"
//#include "KAI/BasePointer.h"
//#include "KAI/PropertyBase.h"
//#include "KAI/MethodBase.h"
////#include "KAI/ClassBase.h"
//#include "KAI/Memory.h"
//#include "KAI/Registry.h"
//#include "KAI/Class.h"
//#include "KAI/Pathname.h"
//#include "KAI/ClassBuilder.h"
//#include "KAI/BuiltinTypes/Void.h" 
//#include "KAI/FunctionBase.h" 
//#include "KAI/Algorithm.h" 
//#include "KAI/Function.h"
//#include "KAI/MethodBase.h" 
//#include "KAI/Method.h"
//
//KAI_BEGIN
//
///// This is an example class to demonstrate how the system
///// reflects C++ type system to the runtime.
/////
///// Note that there are no restrictions on what class can be
///// reflected: it does not have to be derived from a common base.
///// This means that classes that already exist and we do not have
///// access to can be used directly. It also means that builtin
///// types can be used directly.
//class ExampleClass
//{
//public:
//	int Method0()
//	{
//		return 1;
//	}
//	String Method1(int N) const		// const methods are supported
//	{
//		KAI_UNUSED_1(N);
//
//		return "hello";
//	}
//	int Method2(int M, int N)
//	{
//		return N*M;
//	}
//	void Method3()					// methods returning void are supported
//	{
//	}
//	void SetNumber(int N)			
//	{
//		number = N;
//	}
//	void SetString(String N)		// any system type can be used as an argument or return type	
//	{
//		string = N;
//	}
//
//	int number;
//	String string;
//
//	static void Register(Object R)
//	{
//		// this is where we reflect the class type, and the methods we wish
//		// to have exposed to the runtime. notice there are NO MACROS used
//		// in any of the reflection, other than in registering type traits.
//		ClassBuilder<ExampleClass>(*R.GetRegistry(), String("ExampleClass"))
//			.Methods
//			("Method0", &ExampleClass::Method0)
//			("Method1", &ExampleClass::Method1)
//			("Method2", &ExampleClass::Method2)
//			("Method3", &ExampleClass::Method3)
//			("SetNumber", &ExampleClass::SetNumber)
//			("SetString", &ExampleClass::SetString)
//			;
//	}
//};
//
//// before the class can be used, it needs to have its type-traits defined
//// this gives the compile-time system the information it needs to deal with
//// serialisation, operator overloads, etc
////KAI_TYPE_TRAITS(ExampleClass, 321, Properties::StringStreamInsert | Properties::BinaryStreaming);
//KAI_TYPE_TRAITS(ExampleClass, 321, 0);
//
//StringStream &operator<<(StringStream &S, const ExampleClass &Q)
//{
//	return S << "An instance of ExampleClass: number=" << Q.number << ", string=" << Q.string;
//}
//
//BinaryStream &operator<<(BinaryStream &S, const ExampleClass &Q)
//{
//	return S << Q.number << Q.string;
//}
//
//BinaryPacket &operator>>(BinaryPacket &S, ExampleClass &Q)
//{
//	return S >> Q.number >> Q.string;
//}
//
//// a function that will be reflected. the only restraint is that the argument
//// and return types must be known to the system
//int SomeFunction(int N)
//{
//	return N*2;
//}
//
//void AddExamples(Object R)
//{
//	// register the class
//	ExampleClass::Register(R);
//
//	// expose a function to the runtime. again, macros here could help but are not necessary
//	// TODO: why need ::kai::NewFunction???
//	Set(R, Pathname("/Bin/SomeFunction"), ::kai::NewFunction2(R, SomeFunction, Label("SomeFunction")));
//
//	// make a new instance, expose it to the runtime
//	Pointer<ExampleClass> instance = R.GetRegistry()->New<ExampleClass>();
//	instance->number = 123;
//	Set(R, Pathname("/Home/instance"), instance);
//}
//
//KAI_END
//
