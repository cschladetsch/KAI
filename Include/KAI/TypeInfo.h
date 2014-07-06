

#ifndef KAI_TYPE_INFO_H
#define KAI_TYPE_INFO_H

KAI_BEGIN

/// helper structure to switch on system and non-system types
template <class T>
struct TypeInfo
{
	enum { IsSytem = false };	// is this a system type?
	typedef T StorageType;		// the type stored in the parent structure
	typedef T ValueType;		// the underlying value type
};

template <>
struct TypeInfo<Object>
{
	enum { IsSytem = true };
	typedef Object StorageType;
	typedef Object ValueType;
};

template <class T>
struct TypeInfo<Pointer<T> >
{
	enum { IsSytem = true };
	typedef Pointer<T> StorageType;
	typedef T ValueType;
};

KAI_END

#endif // KAI_TYPE_INFO_H

//EOF
