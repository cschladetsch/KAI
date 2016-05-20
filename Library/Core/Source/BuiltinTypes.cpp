#include <algorithm>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/lexical_cast.hpp>

#include <KAI/Core/FunctionBase.h>
#include <KAI/Core/ClassBuilder.h>
#include "KAI/Core/BuiltinTypes/All.h"
#include "KAI/Core/BuiltinTypes/List.h"

KAI_BEGIN

StringStream &operator >> (StringStream &S, float &F)
{
	//return S >> F;
	KAI_NOT_IMPLEMENTED();
}

/* TODO WTF
StringStream &operator<<(StringStream &S, const BasePointerBase &B)
{
	B.Self->GetClass()->Insert(S, *B.Self);
	return S;
}
*/

void BasePointerBase::Register(Registry &R)
{
	ClassBuilder<BasePointerBase>(R, "BasePointerBase")
		;
}

void Handle::Register(Registry &R)
{
	ClassBuilder<Handle>(R, "Handle")
		.Methods
		("GetValue", &Handle::GetValue)
		;
}

void Pair::Register(Registry &R)
{
	ClassBuilder<Pair>(R, "Pair")
		.Methods
		.Properties
		("first", &Pair::first)
		("second", &Pair::second)
		;
}

String FileLocation::ToString(bool strip_path) const
{
	StringStream S;
	std::string loc = file.c_str();
	if (strip_path)
	{
		loc = loc.substr(loc.find_last_of('/') + 1);
		loc = loc.substr(loc.find_last_of('\\') + 1);
	}

	if (!loc.empty())
		S << loc.c_str() << "(" << line << "): ";

	if (!function.Empty())
		S << function << ": ";

	S << Ends;

	return S.ToString();
}
 
template <class Callable>
void WriteArgumentList(StringStream &S, const typename Callable::ArgumentTypes &arguments)
{
	S << "(";
	String sep = "";
	for (const auto &A : arguments)
	{
		S << sep << A.ToString();
		sep = ", ";
	}

	S << ")";
}

String MethodBase::ToString() const
{
	StringStream S;
	S << return_type.ToString() << String(" ") << class_type.ToString() << String("::") << name;
	WriteArgumentList<MethodBase>(S, arguments);

	if (constness == Constness::Const)
		S << " const;";
	else
		S << ";";

	if (!Description.Empty())
		S << " /* " << Description << " */";

	return S.ToString();
}

String FunctionBase::ToString() const
{
	StringStream S;
	S << return_type.ToString() << " " << name;
	WriteArgumentList<FunctionBase>(S, arguments);
	S << ";";

	if (!Description.Empty())
		S << " /* " << Description << " */";

	return S.ToString();
}

void FunctionBase::Register(Registry &R)
{
	ClassBuilder<BasePointer<FunctionBase> >(R, "Function")
		;
}

StringStream &operator<<(StringStream &S, int N)
{
	S << boost::lexical_cast<std::string>(N);
	return S;
}

StringStream &operator>>(StringStream &S, int &N)
{
	String T;
	S >> T;
	N = atoi(T.c_str());
	return S;
}

StringStream &operator<<(StringStream &S, bool B)
{
	if (B)
		S << "true";
	else
		S << "false";

	return S;
}

StringStream &operator<<(StringStream &S, double N)
{
	KAI_UNUSED(N);
	return S;
}

// 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead.
#pragma warning(disable:4996)

StringStream &operator<<(StringStream &S, float N)
{
	char buffer[1000];
	sprintf(buffer, "%f", N);
	return S << buffer;
}

namespace Type
{
	std::string Number::ToString() const
	{
		switch (value)
		{
			#define Case(N) case N : return #N;
			Case(Undefined);
			Case(None);
			Case(List);
			Case(Void);
			Case(Handle);
			Case(Object);
			Case(Type);
			Case(TypeNumber);
			Case(BasePointer);
			Case(Callable);
			Case(Function);
			Case(Method);
			Case(Property);
			Case(Class);
			Case(Bool);
			Case(Signed32);
			Case(Single);
			Case(Double);
			Case(String);
			Case(Label);
			Case(Pathname);
			Case(Array);
			Case(Pair);
			Case(Map);
			Case(HashMap);
			Case(Set);
			Case(HashSet);
			Case(Stack);
			Case(MapConstIterator);
			Case(MapIterator);
			Case(SetConstIterator);
			Case(SetIterator);
			Case(HashMapConstIterator);
			Case(HashMapIterator);
			Case(HashSetConstIterator);
			Case(HashSetIterator);
			Case(Operation);
			Case(Continuation);
			Case(Compiler);
			Case(Executor);
			Case(StreamBase);
			Case(StringStream);
			Case(XmlStream);
			Case(GraphStream);
			Case(BinaryStream);
			Case(BinaryPacket);
			Case(Console);
			Case(Node);
			Case(Browser);
			Case(SignedContinuation);
			Case(Vector2);
			Case(Vector3);
			Case(Vector4);
			Case(Last);
			Case(SystemLast);
		}

		KAI_NAMESPACE(StringStream) S;
		S << "TypeNumber=" << (int)value << Ends;
		return S.ToString().c_str();
	}
}

StringStream &operator<<(StringStream &S, Pair const &P)
{
	return S << "Pair(" << P.first << ", " << P.second << ")";
}

StringStream &operator>>(StringStream &S, Pair &P)
{
	KAI_UNUSED_2(S,P);
	KAI_NOT_IMPLEMENTED();
}

BinaryStream &operator<<(BinaryStream &S, Pair const &P)
{
	return S << P.first << P.second;
}

BinaryPacket &operator>>(BinaryPacket &S, Pair &P)
{
	return S >> P.first >> P.second;
}

HashValue GetHash(const Pair &P)
{
	return GetHash(P.first);
}

StringStream &operator<<(StringStream &S, Type::Number T)
{
	return S << T.ToString();//"TypeNumber: " << T.ToInt() << "(" << T.ToString() << ")";
}

StringStream &operator<<(StringStream &S, BasePointer<PropertyBase> const &P)
{
	return S << "Property: name=" << P->GetFieldName() << ", type=" << P->GetFieldTypeNumber() << ", class=" << P->GetClassTypeNumber();
}

StringStream &operator<<(StringStream &S, Vector2 const &V)
{
	return S << V.x << " " << V.y;
}

StringStream &operator>>(StringStream &S, Vector2 &V)
{
	KAI_UNUSED_2(S,V);
	KAI_NOT_IMPLEMENTED();
}

BinaryStream &operator<<(BinaryStream &S, Vector2 const &V)
{
	return S << V.x << V.y;
}

BinaryPacket &operator>>(BinaryPacket &S, Vector2 &V)
{
	return S >> V.x >> V.y;
}

StringStream &operator<<(StringStream &S, Vector3 const &V)
{
	return S << V.x << " " << V.y << " " << V.z;
}

StringStream &operator>>(StringStream &S, Vector3 &V)
{
	return S >> V.x >> V.y >> V.z;
}

BinaryStream &operator<<(BinaryStream &S, Vector3 const &V)
{
	return S << V.x << V.y << V.z;
}

BinaryPacket &operator>>(BinaryPacket &S, Vector3 &V)
{
	return S >> V.x >> V.y >> V.z;
}

StringStream &operator<<(StringStream &S, Vector4 const &V)
{
	return S << V.x << " " << V.y << " " << V.x << " " << V.w;
}

StringStream &operator>>(StringStream &, Vector4 &)
{
	KAI_NOT_IMPLEMENTED();
}

BinaryStream &operator<<(BinaryStream &S, Vector4 const &V)
{
	return S << V.x << V.y << V.z << V.w;
}

BinaryPacket &operator>>(BinaryPacket &S, Vector4 &V)
{
	return S >> V.x >> V.y >> V.z >> V.w;
}

StringStream &operator<<(StringStream &S, std::size_t N)
{
	return S << (int)N;
}

StringStream &operator<<(StringStream &S, Handle H)
{
	return S << "Handle: " << H.GetValue();
}

KAI_END

//EOF
