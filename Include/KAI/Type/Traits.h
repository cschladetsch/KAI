#pragma once

KAI_TYPE_BEGIN

template <class T>
struct NoTraitsDefined;

template <class T>
struct Traits
{
	typedef typename NoTraitsDefined<T>::ForType Type;
};

template <>
struct Traits<meta::Null>
{
	enum { Num = NumberEnum::None };
	enum { Properties = 0 };

	typedef meta::Null Type;
	typedef meta::Null Store;
	typedef meta::Null *Pointer;
	typedef meta::Null const *ConstPointer;
	typedef meta::Null &Reference;
	typedef meta::Null const &ConstReference;	
	static const char *Name;
};

template <class T>
struct Traits<const T> : Traits<T>
{
};

template <class T>
struct StorageType
{
	typedef T Type;
};

//template <class T>
//struct StorageType<FwdBasePointer<T> >
//{
//	typedef T Type;
//};

template <typename Reference, typename Parent>
struct UpCast
{
	static Storage<Parent *> *Cast(Registry &R, Reference Q)
	{
		Storage<Parent *> *parent = NewStorage<Parent *>(R);
		**parent = &Q;
		return parent;
	}
};
template <typename Reference>
struct UpCast<Reference, meta::Null>
{
	static Storage<meta::Null> *Cast(Registry &, Reference)
	{
		return 0;
	}
};

// Assignment
template <typename Reference, typename ConstReference, int Number, bool>
struct AssignOp
{
	static void Perform(Reference A, ConstReference B)
	{
		A = B;
	}
};
template <typename Reference, typename ConstReference, int Number>
struct AssignOp<Reference, ConstReference, Number, false>
{
	static void Perform(Reference, ConstReference)
	{
		KAI_THROW_2(NoProperty, Number, Properties::Assign);
	}
};

// absolute
template <typename Reference, typename ConstReference, int Number, bool>
struct AbsoluteOp
{
	static void Perform(Reference value)
	{
		SetAbsoluteValue(value);
	}
};
template <typename Reference, typename ConstReference, int Number>
struct AbsoluteOp<Reference, ConstReference, Number, false>
{
	static void Perform(Reference)
	{
		KAI_THROW_2(NoProperty, Number, Properties::Absolute);
	}
};

// Less
template <typename C, int N, bool>
struct LessOp
{
	static bool Perform(C A, C B)
	{
		return A < B;
	}
};
template <typename C, int N>
struct LessOp<C, N, false>
{
	static bool Perform(C, C)
	{
		KAI_THROW_2(NoProperty, N, Properties::Less);
	}
};

/// Equiv
template <typename C, int N, bool>
struct EquivOp
{
	static bool Perform(C A, C B)
	{
		return A == B;
	}
};
template <typename C, int N>
struct EquivOp<C, N, false>
{
	static bool Perform(C, C)
	{
		KAI_THROW_2(NoProperty, N, Properties::Equiv);
	}
};
/// Greater
template <typename C, int N, bool>
struct GreaterOp
{
	static bool Perform(C A, C B)
	{
		return A > B;
	}
};
template <typename C, int N>
struct GreaterOp<C, N, false>
{
	static bool Perform(C, C)
	{
		KAI_THROW_2(NoProperty, N, Properties::Greater);
	}
};


// TODO: implement arithmetic using += etc
template <typename Store, typename C, int N, bool = true>
struct PlusOp
{
	static Store Perform(C A, C B)
	{
		return A + B;
	}
};
template <typename Store, typename C, int N>
struct PlusOp<Store, C, N, false>
{
	static Store Perform(C, C)
	{
		KAI_THROW_2(NoProperty, N, Properties::Plus);
	}
};

// minus
template <typename Store, typename C, int N, bool = true>
struct MinusOp
{
	static Store Perform(C A, C B)
	{
		return A - B;
	}
};
template <typename Store, typename C, int N>
struct MinusOp<Store, C, N, false>
{
	static Store Perform(C, C)
	{
		KAI_THROW_2(NoProperty, N, Properties::Minus);
	}
};


template <typename Store, typename C, int N, bool = true>
struct MultiplyOp
{
	//BOOST_STATIC_ASSERT((N & Properties::Multiply) == 0);

	static Store Perform(C A, C B)
	{
		return A * B;
	}
};
template <typename Store, typename C, _int64 N>
struct MultiplyOp<Store, C, N, false>
{
	//BOOST_STATIC_ASSERT((N & Properties::Multiply) != 0);
	static Store Perform(C, C)
	{
		KAI_THROW_2(NoProperty, N, Properties::Multiply);
	}
};

template <typename Store, typename C, int N, bool = true>
struct DivideOp
{
	static Store Perform(C A, C B)
	{
		return A / B;
	}
};
template <typename Store, typename C, int N>
struct DivideOp<Store, C, N, false>
{
	static Store Perform(C, C)
	{
		KAI_THROW_2(NoProperty, N, Properties::Divide);
	}
};

template <typename C, int N, bool = true>
struct HashOp
{
	static HashValue Calc(C A)
	{
#pragma warning (push)
		// warning: unreachable code. NFI why...
#pragma warning (disable:4702)
		return GetHash(A);
#pragma warning (push)
	}
};
template <typename C, int N>
struct HashOp<C, N, false>
{
	static HashValue Calc(C)
	{
		KAI_THROW_2(NoProperty, N, Properties::CalcHashValue);
	}
};

template <class T, enum NumberEnum, enum Properties Props, class P = None, class St = typename StorageType<T>::Type, class Ref = T &, class ConstRef = T const &>
struct TraitsBase
{
	enum { Number = NumberEmum };
	enum { Props = PropertiesEnum };
	typedef T Type;
	typedef P Parent;
	typedef St Store;
	typedef Store *Pointer;
	typedef Store const *ConstPointer;
	typedef Ref Reference;
	typedef ConstRef ConstReference;

	typedef UpCast<Reference, Parent> UpCaster;
	
	template <enum Properties N2>
	struct HasProperty
	{
		enum
		{
			Value = (Ops & std::underlying_type<Properties>::type(N2)) != 0
		};
	};

	template <enum Properties N3>
	struct HasProperties { enum { Value = (Ops & (int)N3) == N3 }; };

	struct UnReflectedLifetimeManagement
	{
		static void Create(Storage<T> *)
		{
		}
		static bool Destroy(Storage<T> *)
		{
			return true;
		}
		static void Delete(Storage<T> *ptr)
		{
			if (ptr)
				ptr->GetRegistry()->GetMemorySystem().DeAllocate(ptr);
		}
	};

	struct ReflectedLifetimeManagement
	{
		static void Create(Storage<T> *storage)
		{
			UnReflectedLifetimeManagement::Create(storage);
			ReflectedBase *reflected = &storage->GetCleanReference();
			reflected->Self = storage;
			reflected->Create();
		}
		static bool Destroy(Storage<T> *storage)
		{
			ReflectedBase *reflected = &storage->GetReference();
			bool destroyed = reflected->Destroy();
			if (destroyed)
				UnReflectedLifetimeManagement::Destroy(storage);
			return destroyed;
		}
		static void Delete(Storage<T> *storage)
		{
			ReflectedBase *reflected = &storage->GetReference();
			reflected->Delete();
			UnReflectedLifetimeManagement::Delete(storage);
		}
	};

	typedef typename meta::If<HasProperty<Properties::Reflected>::Value
		, ReflectedLifetimeManagement
		, UnReflectedLifetimeManagement>::Type 
		LifetimeManager;

	typedef AssignOp<Reference, ConstReference, Number, HasProperty<Properties::Assign>::Value> Assign;
	typedef AbsoluteOp<Reference, ConstReference, Number, HasProperty<Properties::Absolute>::Value> Absolute;

	typedef LessOp<ConstReference, Number, HasProperty<Properties::Less>::Value> Less;
	typedef EquivOp<ConstReference, Number, HasProperty<Properties::Equiv>::Value> Equiv;
	typedef GreaterOp<ConstReference, Number, HasProperty<Properties::Greater>::Value> Greater;

	typedef PlusOp<St, ConstReference, HasProperty<Properties::Plus>::Value> Plus;
	typedef MinusOp<St, ConstReference, HasProperty<Properties::Minus>::Value> Minus;
	typedef DivideOp<St, ConstReference, HasProperty<Properties::Divide>::Value> Divide;
	// MUL typedef MultiplyOp<St, ConstReference, HasProperty<Properties::Multiply>::Value> Multiply;

	template <bool, class Stream>
	struct StreamInsertOpt
	{
		static void Insert(Stream &S, ConstReference X)
		{
			S << X;
		}
	};
	template <class Stream>
	struct StreamInsertOpt<false, Stream>
	{
		static void Insert(Stream &, ConstReference)
		{
			KAI_THROW_2(NoProperty, Number, Properties::StreamInsert);
		}
	};
	template <bool, class Stream>
	struct StreamExtractOpt
	{
		static void Extract(Stream &S, Reference X)
		{
			S >> X;
		}
	};
	template <class Stream>
	struct StreamExtractOpt<false, Stream>
	{
		static void Extract(Stream &, ConstReference)
		{
			KAI_THROW_2(NoProperty, Number, Properties::StreamExtract);
		}
	};
	typedef StreamInsertOpt<HasProperty<Properties::StringStreamInsert>::Value, ::kai::StringStream> StringStreamInsert;
	typedef StreamExtractOpt<HasProperty<Properties::StringStreamExtract>::Value, ::kai::StringStream> StringStreamExtract;
	typedef StreamInsertOpt<HasProperty<Properties::BinaryStreamInsert>::Value, ::kai::BinaryStream> BinaryStreamInsert;
	typedef StreamExtractOpt<HasProperty<Properties::BinaryStreamExtract>::Value, ::kai::BinaryPacket> BinaryPacketExtract;
	//typedef typename StreamInsert<HasProperty<Properties::XmlStreamInsert>::Value, XmlStream> XmlStreamInsert;

	typedef HashOp<ConstReference, Number, !HasProperty<Properties::NoHashValue>::Value> HashFunction;
};

KAI_TYPE_END

KAI_BEGIN

::std::size_t GetHash(const ::kai::String &);

template <class T>
struct DerefType
{
	typedef T Value;
	typedef typename Type::Traits<T>::Reference Reference;
	typedef typename Type::Traits<T>::ConstReference ConstReference;
};

template <class T>
Storage<T> *Clone(StorageBase const &);

template <class T>
typename DerefType<T>::Reference Deref(StorageBase &);

template <class T>
typename DerefType<T>::ConstReference ConstDeref(StorageBase const &);

KAI_END

namespace boost
{
	/*
	inline size_t hash_value(KAI_NAMESPACE(Handle) const &H)
	{
		throw;
		//return H.GetValue();
	}

	inline size_t hash_value(::kai::NumberEnum const &H)
	{
		throw;
		//return H.GetValue();
	}
	*/
}

