#include <KAI/Core/Base.h>
#include <KAI/Core/FwdDeclarations.h>

KAI_BEGIN

inline StringStream &operator<<(StringStream &S, const String::Char c)
{
	S.Append(c);
	return S;
}

inline StringStream &operator<<(StringStream &S, const Exception::Base &E) 
{ 
	return S << E.ToString().c_str(); 
}

inline StringStream& operator<<(StringStream &S, ObjectColor::Color c)
{
	return S << "ObjColor(" << (int)c << ')';
}

inline bool operator==(StringStream const &A, StringStream const &B)
{
	return A.GetStorage() == B.GetStorage();
}

inline bool operator!=(StringStream const &A, StringStream const &B)
{
	return A.GetStorage() != B.GetStorage();
}

inline bool operator<(StringStream const &A, StringStream const &B)
{
	return A.GetStorage() < B.GetStorage();
}

KAI_TYPE_TRAITS(
	StringStream,
	Number::StringStream,
	Properties::Equiv// || Properties::StringStreaming
	);

KAI_END

