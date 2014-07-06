

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_OBJECT_CONSTRUCT_PARAMS_H
#	define KAI_OBJECT_CONSTRUCT_PARAMS_H

KAI_BEGIN

struct ObjectConstructParams
{
	const ClassBase *class_base;
	Registry *registry;
	Handle handle;
	Constness constness;

	ObjectConstructParams() : class_base(0), registry(0) { }
	ObjectConstructParams(Registry *, const ClassBase *, Handle, Constness = Constness::Mutable);
	ObjectConstructParams(const Object &, Constness);
	ObjectConstructParams(const StorageBase *);
	ObjectConstructParams(StorageBase *);
};

KAI_END

#endif // KAI_OBJECT_CONSTRUCT_PARAMS_H

//EOF

