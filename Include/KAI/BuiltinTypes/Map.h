#pragma once

#include "KAI/BuiltinTypes/MapBase.h"

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

KAI_TYPE_TRAITS(Map::const_iterator, Number::MapConstIterator
	, Properties::StringStreamInsert
	| Properties::Equiv
	//| Properties::Container ?? should an iterator be considered a container?
	);

KAI_END
