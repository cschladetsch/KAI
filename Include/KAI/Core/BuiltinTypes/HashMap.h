#pragma once

#include <unordered_map>

KAI_BEGIN

class Object;

/// A Map defines an associative container. Because this maps Object to Object,
/// it is possible to store mappings of any type to any type.

struct HashMapCompare
{
	// TODO: determine optimal values for these
	enum { bucket_size = 333, min_buckets = 3 };
	std::size_t operator()(const Object &A) const;
	bool operator()(const Object &A, const Object &B) const;
};

typedef MapBase<std::unordered_map<Object, Object, HashMapCompare> > HashMap;

KAI_TYPE_TRAITS(HashMap, Number::HashMap
	, Properties::StringStreamInsert 
	| Properties::BinaryStreaming 
	| Properties::Equiv 
	| Properties::Less
	| Properties::Assign
	| Properties::Reflected
	| Properties::Container);

KAI_END
