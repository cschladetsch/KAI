#include "KAI/KAICommon.h"

#ifdef KAI_UNIT_TESTS
#	include "KAI/Tests/TestCompiler.h"
#	include "KAI/Tests/TestObject.h"
#	include "KAI/Tests/TestMethod.h"
#	include "KAI/Tests/TestFunction.h"
#	include "KAI/Tests/TestProperties.h"
#	include "KAI/Tests/TestRegistry.h"
#	include "KAI/Tests/TestTriColor.h"
#	include "KAI/Tests/TestMap.h"
#	include "KAI/Tests/TestEvents.h"
#	include "KAI/Tests/TestTree.h"
#	include "KAI/Tests/TestPathname.h"
#	include "KAI/Tests/TestBinaryStream.h"
#	include "KAI/Tests/TestStringStream.h"
#	include "KAI/Tests/TestDotGraph.h"
#	include "KAI/Tests/TestObjectIndexing.h"
#	include "KAI/Tests/TestExample.h"
#	include "KAI/Tests/TestExecutor.h"
#	include "KAI/Tests/TestTest.h"
#endif

#include <iostream>
#include <fstream>

#include "KAI/BuiltinTypes/Container.h"
#include "KAI/BuiltinTypes/Array.h"
#include "KAI/BuiltinTypes/Stack.h"
#include "KAI/BuiltinTypes/String.h"
#include "KAI/BuiltinTypes/Signed32.h"
#include "KAI/BuiltinTypes/Bool.h"

#include "KAI/Reflected.h"
#include "KAI/Pointer.h"
#include "KAI/Continuation.h"
#include "KAI/Operation.h"
#include "KAI/Pathname.h"
#include "KAI/Executor.h"

#include "KAI/Compiler.h"
#include "KAI/Value.h"
#include "KAI/BasePointer.h"
#include "KAI/CallableBase.h"
#include "KAI/MethodBase.h"
#include "KAI/Exception/Extended.h"
#include "KAI/PropertyBase.h"
#include "KAI/ClassBase.h"

#include "KAI/BuiltinTypes/Vector4.h"
#include "KAI/StringStream.h"
#include "KAI/BinaryPacket.h"
#include "KAI/BinaryStream.h"

#include "KAI/GetStorageBase.h"

KAI_BEGIN

using namespace std;

bool ExecuteFile(const char *filename, Pointer<Executor> executor, Pointer<Compiler> compiler, Object scope);

namespace Bin
{
	Object GetMethods(Object Q)
	{
		if (!Q.Exists())
			return Object();

		Value<Array> methods = Q.New<Array>();
		const ClassBase *K = Q.GetClass();
		//ClassBase::Methods::const_iterator A = K->GetMethods().begin(), B = K->GetMethods().end();
		//for (; A != B; ++A)
		for (auto m : K->GetMethods())
			methods->Append(Q.New<BasePointer<MethodBase> >(m.second));

		return methods.GetObject();
	}

	void Help()
	{
		cout << "There is no help" << endl;
	}

	Object GetProperties(Object Q)
	{
		if (!Q.Exists())
			return Object();

		Value<Array> properties = Q.New<Array>();
		for (auto prop : Q.GetClass()->GetProperties())
			properties->Append(Q.New<BasePointer<PropertyBase>>(prop.second));

		return properties.GetObject();
	}

	Vector3 ScaleVector3(Vector3 vec, float scalar)
	{
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	Vector3 AddVector3(Vector3 vec, Vector3 addition)
	{
		return Vector3(vec.x + addition.x, vec.y + addition.y, vec.z + addition.z);
	}

	void WriteToFile(String filename, Object Q)
	{
		std::fstream file(filename.c_str(), std::ios::out);
		file << Q.ToString().c_str();
		file.close();
	}

	String ReadFile(String filename)
	{
		std::fstream file(filename.c_str(), std::ios::in);
		if (!file)
			KAI_THROW_1(FileNotFound, filename);

		char line[2000];
		StringStream text;
		while (file.getline(line, 2000))
		{
			text.Append(line);
			text.Append('\n');
		}

		file.close();
		return text.ToString();
	}

	void Printf(String fmt, Array items)
	{
		for (int n = 0; n < fmt.size(); ++n)
		{
			char ch = fmt[n];
			if (ch == '#')
			{
				int num = fmt[++n] - '0';
				std::cout << items.At(num);
				continue;
			}

			std::cout << ch;
		}
	}

	void Print(Object Q)
	{
		std::cout << Q.ToString().c_str();
	}

	void PrintXml(Object Q)
	{
		std::cout << Q.ToXmlString().c_str() << std::endl;
	}

	Object UpCast(Object Q)
	{
		if (!Q.Exists())
			return Object();

		return Q.GetClass()->UpCast(Q.GetStorageBase());
	}

	void SetClean(Object Q, bool D)
	{
		if (D)
			Q.SetClean();
		else
			Q.SetClean(false);
	}

	bool IsDirty(Object Q)
	{
		return Q.IsDirty();
	}

	bool IsConst(Object Q)
	{
		return Q.IsConst();
	}

	void Assert(bool B)
	{
		if (!B)
			KAI_THROW_0(AssertionFailed);
	}

	Object Freeze(Object Q)
	{
		Value<BinaryStream> S = Q.New<BinaryStream>();
		*S << Q;
		return S.GetObject();
	}

	Object Thaw(Object Q)
	{
		BinaryStream &S = Deref<BinaryStream>(Q);
		S.SetRegistry(Q.GetRegistry());
		Object N;
		S >> N;
		return N;
	}

	String ToString(Object Q)
	{
		return Q.ToString();
	}

	String ToXmlString(Object Q)
	{
		return Q.ToXmlString();
	}

	void ExitToOS(int N)
	{
		exit(N);
	}

#if KAI_RUN_TESTS

	Object RunOne(Object object)
	{
		Test::Module module;
		module.AddSuite<Tests::TestExample>("TestExample");
	
		Pointer<Test::BasicOutput> out = object.New<Test::BasicOutput>();
		
		module.Run(out);

		return object.New(out->GetSummary());
	}

	#define ADD_TEST(Name) \
		module.AddSuite<Tests::Name>(#Name);

	Object RunAllTests(Object object)
	{
		Test::Module module;
		ADD_TEST(TestBinaryStream);
		ADD_TEST(TestCompiler);
		//ADD_TEST(TestDerivation);
		ADD_TEST(TestDotGraph);
		ADD_TEST(TestEvents);
		ADD_TEST(TestExample);
		ADD_TEST(TestExecutor);
		ADD_TEST(TestMap);
		ADD_TEST(TestMethod);
		ADD_TEST(TestObject);
		ADD_TEST(TestProperties);
		ADD_TEST(TestRegistry);
		ADD_TEST(TestStringStream);
		ADD_TEST(TestPathname);
		ADD_TEST(TestTree);
		ADD_TEST(TestTriColor);

		Pointer<Test::BasicOutput> out = object.New<Test::BasicOutput>();
		//Pointer<Test::XmlOutput> out = object.New<Test::XmlOutput>();
		module.Run(out);
		return object.New(Pair(out, object.New(out->GetSummary())));
		//return object.New(out->ToString());
	}

	/*Pointer<ClassBase const *> */Object TypeNumberToClass(Object tn)
	{
		Pointer<int> type_number = tn;
		return type_number.New(type_number.GetRegistry()->GetClass(*type_number));
	}

	/// returns a pair containing the methods and properties of a class
	/*Pointer<Pair>*/ Object Describe(Object Q)
	{
		Pointer<Pair> P = Q.New<Pair>();
		P->first = GetMethods(Q);
		P->second = GetProperties(Q);
		return P;
	}

	String info(Object object)
	{
		StringStream text;
		text << "Methods:\n";
		foreach (Object const &method, ConstDeref<Array>(GetMethods(object)))
			text << "\t" << method.ToString() << "\n";

		text << "Properties:\n";
		foreach (Object const &property, ConstDeref<Array>(GetProperties(object)))
			text << "\t" << property.ToString() << "\n";

		return text.ToString();
	}

	void AddFunctions(Object Q)
	{
		#define ADD_FUN(N, D) \
			AddFunction(Q, N, #N, D);
		ADD_FUN(TypeNumberToClass, "Returns the Class of a given type number");
		ADD_FUN(GetMethods, "Returns an Array of all Methods for a given Object");
		ADD_FUN(GetProperties, "Returns an Array of all Properties for a given Object");
		ADD_FUN(Describe, "Returns Methods and Properties for a given Object");
		ADD_FUN(info, "Prints the methods and properties");
		AddFunction(Q, ExitToOS, "Exit", "Ends the Session");
		ADD_FUN(Assert, "Throws AssertionFailed if Object equates to False");
		ADD_FUN(Freeze, "Inserts Object into a BinaryStream");
		ADD_FUN(Thaw, "Extracts an Object from a BinarySteam");
		ADD_FUN(IsDirty, "Returns True if Object is Dirty");
		ADD_FUN(IsConst, "Returns True if Object is Const");
		ADD_FUN(SetClean, "");
		ADD_FUN(ToString, "Returns a human-readable String reprentation of an Object");
		ADD_FUN(ToXmlString, "Returns an XML String reprentation of an Object");
		ADD_FUN(UpCast, "Returns a Pointer to the Base of an Object");
		ADD_FUN(Print, "Prints an Object to stdout");
		ADD_FUN(Printf, "Print a list of items");
		ADD_FUN(PrintXml, "Print's an Object to stdout in XML");
		ADD_FUN(WriteToFile, "Writes an Object to a text file");
		ADD_FUN(ReadFile, "Reads a text file");
		ADD_FUN(ScaleVector3, "Multiply a vector by a float");
		ADD_FUN(AddVector3, "Adds two Vector3s together.")
		ADD_FUN(Help, "LOL");
		
#ifdef KAI_UNIT_TESTS
		AddFunction(Q, RunAllTests, "tests", "run all tests");
		AddFunction(Q, RunOne, "test", "run current test");
#endif
	}
#endif
}

KAI_END