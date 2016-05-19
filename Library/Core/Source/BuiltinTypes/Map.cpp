#include <KAI/Core/BuiltinTypes/MapBase.h>
#include "KAI/Core/Object.h".h"
#include "KAI/Core/BuiltinTypes/HashMap.h"
#include "KAI/Core/BuiltinTypes/Map.h"
#include "KAI/Core/BuiltinTypes/List.h"
#include "KAI/Core/KAIClassBuilder.h"

KAI_BEGIN

StringStream &operator<<(StringStream &S, typename Map::const_iterator const &P)
{
	KAI_UNUSED(P);
	return S << "MapConstIterator";
}

std::size_t HashMapCompare::operator()(const Object &A) const
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
