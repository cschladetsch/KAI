#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_BUILTIN_TYPES_MAP_H
#	define KAI_BUILTIN_TYPES_MAP_H

KAI_BEGIN

struct MapCompare
{
	bool operator()(const Object &A, const Object &B) const;
};

typedef MapBase<nstd::map<Object, Object, MapCompare> > Map;

KAI_TYPE_TRAITS(Map, Type::Number::Map
	, Type::Properties::StringStreamInsert 
	| Type::Properties::BinaryStreaming 
	| Type::Properties::Equiv 
	// CJS TODO | Type::Properties::Less
	| Type::Properties::Reflected
	| Type::Properties::Container);

StringStream &operator<<(StringStream &, Map::const_iterator const &);

KAI_TYPE_TRAITS(Map::const_iterator, Type::Number::MapConstIterator
	, Type::Properties::StringStreamInsert
	| Type::Properties::Equiv
	//| Type::Properties::Container ?? should an iterator be considered a container?
	);


KAI_END

#endif // KAI_BUILTIN_TYPES_MAP_H

//EOF

