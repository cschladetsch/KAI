// (C) 2014 christian.schladetsch@gmail.com

#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_GET_OBJECT_BASE_H
#	define KAI_GET_OBJECT_BASE_H

KAI_BEGIN

#ifndef _DEBUG
__forceinline
#endif
StorageBase &GetStorageBase(Object const &Q);

Type::Number GetTypeNumber(Object const &Q);

template <class T>
Storage<T> &GetStorage(Pointer<T> const &P)
{
	return GetStorage<T>(GetStorageBase(P));
}

template <class T>
Storage<T> &GetStorage(StorageBase &base)
{
	if (base.GetTypeNumber().GetValue() != Type::Traits<T>::Number)
		KAI_THROW_2(TypeMismatch, base.GetTypeNumber().ToInt(), Type::Traits<T>::Number);
	return static_cast<Storage<T> &>(base);
}

template <class T>
typename DerefType<T>::Reference Deref(StorageBase &base)
{
	if (base.IsConst())
		KAI_THROW_0(ConstError);
	return GetStorage<DerefType<T>::Value>(base).GetReference();
}

template <class T>
typename T &Deref(T &Q)
{
	return Q;
}

template <class T>
typename T const &Deref(T const &Q)
{
	return Q;
}

template <class T>
typename DerefType<T>::Reference CleanDeref(StorageBase &base)
{
	return GetStorage<DerefType<T>::Value>(base).GetCleanReference();
}

template <class T>
typename DerefType<T>::Reference Deref(Object const &Q)
{
	return Deref<T>(GetStorageBase(Q));
}

template <class T>
ConstStorage<T> const &GetConstStorage(StorageBase const &base)
{
	if (base.GetTypeNumber() != Type::Traits<T>::Number)
		KAI_THROW_2(TypeMismatch, base.GetTypeNumber().ToInt(), Type::Traits<T>::Number);
	return static_cast<ConstStorage<T> const &>(base);
}

template <class T>
typename DerefType<T>::ConstReference ConstDeref(StorageBase const &base)
{
	return GetConstStorage<DerefType<T>::Value>(base).GetConstReference();
}

template <class T>
typename DerefType<T>::ConstReference ConstDeref(const Object &Q)
{
	return ConstDeref<T>(GetStorageBase(Q));
}

template <>
inline Object &Deref<Object>(Object const &Q)
{
	return const_cast<Object &>(Q);
}

template <>
inline const Object &ConstDeref<Object>(const Object &Q)
{
	return Q;
}

StorageBase &GetStorageBase(Object const &Q);

template <class T>
Storage<T> *Clone(StorageBase const &Q)
{
	auto dup = Q.GetRegistry()->NewStorage<T>();
	dup->GetClass()->Clone(*dup, Q);
	return dup;
}

KAI_END

#endif // KAI_GET_OBJECT_BASE_H
