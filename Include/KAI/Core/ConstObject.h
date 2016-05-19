

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_CONST_OBJECT_H
#	define KAI_CONST_OBJECT_H

KAI_BEGIN

struct ConstObject : Object
{
	ConstObject();
	ConstObject(Object const &);
	ConstObject(ConstObject const &);
	ConstObject& operator=(Object const &);
	ConstObject& operator=(ConstObject const &);

	StorageBase const &GetStorageBase() const;
	Dictionary const &GetDictionary() const;
	ConstObject GetChild(Label const &) const;
	template <class T>
	ConstObject GetValue(Label const &) const;
};

KAI_END

#endif // KAI_CONST_OBJECT_H

//EOF
