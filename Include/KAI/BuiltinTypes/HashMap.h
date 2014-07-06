
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_BUILTIN_TYPES_HASH_MAP_H
#	define KAI_BUILTIN_TYPES_HASH_MAP_H

KAI_BEGIN

/// A Map defines an associative container. Because this maps Object to Object,
/// it is possible to store mappings of any type to any type.

struct HashMapCompare
{
	// TODO: determine optimal values for these
	enum { bucket_size = 333, min_buckets = 3 };
	nstd::size_t operator()(const Object &A) const;
	bool operator()(const Object &A, const Object &B) const;
};

typedef MapBase<nstd::hash_map<Object, Object, HashMapCompare> > HashMap;

KAI_TYPE_TRAITS(HashMap, Type::Number::HashMap
	, Type::Properties::StringStreamInsert 
	| Type::Properties::BinaryStreaming 
	| Type::Properties::Equiv 
	| Type::Properties::Less
	| Type::Properties::Assign
	| Type::Properties::Reflected
	| Type::Properties::Container);

KAI_END

#endif // KAI_BUILTIN_TYPES_HASH_MAP_H

//EOF

