#pragma once

#include <map>

#include <KAI/Core/Config/Base.h>
#include "KAI/Core/BuiltinTypes/MapBase.h"
#include <KAI/Core/StringStream.h>
#include <KAI/Core/Type/Type.h>

KAI_BEGIN

struct MapCompare
{
	bool operator()(const Object &A, const Object &B) const;
};

typedef MapBase<std::map<Object, Object, MapCompare> > Map;

KAI_TYPE_TRAITS(Map, Number::Map
	, Properties::StringStreamInsert 
	| Properties::BinaryStreaming 
	| Properties::Equiv 
	// CJS TODO | Properties::Less
	| Properties::Reflected
	| Properties::Container);

StringStream &operator<<(StringStream &, Map::const_iterator const &);

/* TODO or not?
KAI_TYPE_TRAITS(Map::const_iterator, Number::MapConstIterator
	, Properties::StringStreamInsert
	| Properties::Equiv
	//| Properties::Container ?? should an iterator be considered a container?
	);
	*/

KAI_END

