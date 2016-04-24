
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#include <hash_map>

#ifndef KAI_BUILTIN_TYPES_HASH_MAP_H
#	define KAI_BUILTIN_TYPES_HASH_MAP_H

KAI_BEGIN

struct Object;

/// A Map defines an associative container. Because this maps Object to Object,
/// it is possible to store mappings of any type to any type.

struct HashMapCompare
{
	// TODO: determine optimal values for these
	enum { bucket_size = 333, min_buckets = 3 };
	nstd::size_t operator()(const Object &A) const;
	bool operator()(const Object &A, const Object &B) const;
};

typedef MapBase<std::hash_map<Object, Object, HashMapCompare> > HashMap;

KAI_TYPE_TRAITS(HashMap, Number::HashMap
	, Properties::StringStreamInsert 
	| Properties::BinaryStreaming 
	| Properties::Equiv 
	| Properties::Less
	| Properties::Assign
	| Properties::Reflected
	| Properties::Container);

KAI_END

#endif // KAI_BUILTIN_TYPES_HASH_MAP_H

//EOF

