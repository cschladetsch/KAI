
#include "KAI/KAI.h"
#include "KAI/BuiltinTypes/Signed32.h"
#include "KAI/BuiltinTypes/Bool.h"
#include "KAI/BuiltinTypes/MapBase.h"
#include "KAI/BuiltinTypes/HashMap.h"
#include "KAI/BuiltinTypes/Map.h"
#include "KAI/BuiltinTypes/List.h"

KAI_BEGIN

//#define KAI_PRAGMA_MESSAGE_NOT_IMPLEMENTED() \
//	message 

StringStream &operator<<(StringStream &S, Map::const_iterator const &P)
{
	KAI_UNUSED(P);
	return S << "MapConstIterator";
}

nstd::size_t HashMapCompare::operator()(const Object &A) const
{
	if (!A.Valid())
		return 0;
	return GetHash(A);
}

bool HashMapCompare::operator()(const Object &A, const Object &B) const
{
	if (!A.Valid())
		return B.Valid();
	if (!A.Exists())
		return B.Exists();

	return GetHash(A) == GetHash(B);
}

bool MapCompare::operator()(const Object &A, const Object &B) const
{
	if (!A.Valid())
		return B.Valid();
	if (!A.Exists())
		return B.Exists();

	bool same_class = A.GetClass() == B.GetClass();
	if (same_class && A.GetClass()->HasTraitsProperty(Type::Properties::Less))
		return A.GetClass()->Less(A.GetStorageBase(), B.GetStorageBase());
	
	return GetHash(A) < GetHash(B);
}

KAI_END

//EOF
