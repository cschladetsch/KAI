#pragma once

KAI_BEGIN

template <class T>
struct DerefType
{
	typedef T Value;
	typedef typename Type::Traits<T> Tr;
	typedef typename Tr::Reference Reference;
	typedef typename Tr::ConstReference ConstReference;
};

template <class T>
Storage<T> *Clone(StorageBase const &);

template <class T>
typename DerefType<T>::Reference Deref(StorageBase &);

template <class T>
typename DerefType<T>::ConstReference ConstDeref(StorageBase const &);

::std::size_t GetHash(const ::kai::String &);

KAI_END

