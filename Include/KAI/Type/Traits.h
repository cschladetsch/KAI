#pragma once

#include <boost/type_index.hpp>

KAI_TYPE_BEGIN

template <typename T>
struct NoTraitsDefined;

template <typename T>
struct Traits
{
	typedef typename NoTraitsDefined<T>::ForType Type;
};

typedef int TypeNumber;

template <typename T>
struct Traits<const T> : Traits<T>
{
};

template <class T>
struct StorageType
{
	typedef T Type;
};

template <class T, int E, int Q, class St = typename StorageType<T>::Type, class Ref = T &, class ConstRef = T const &>
struct TraitsBase
{
	enum { Number = (E) };
	enum { Props = (Q) };

	typedef T Type;
	typedef St Store;
	typedef Store *Pointer;
	typedef Store const *ConstPointer;
	typedef Ref Reference;
	typedef ConstRef ConstReference;


	static const char *Name()
	{
		return boost::typeindex::type_id<T>().pretty_name().c_str();
	}

	// used as a dummy template parameter to avoid nested explicit template instantiations.
	class D { };

	template <int N2>
	struct HasProperty { enum { Value = (N2 & Q) != 0 }; };

	template <int N3>
	struct HasProperties { enum { Value = (N3 & Q) == N3 }; };

	template <class, bool>
	struct UpCast
	{
		//static Storage<Parent *> *Cast(Registry &R, Reference Q)
		//{
		//	return 0;
		//}
	};

	template <class X>
	struct UpCast <X, true>
	{
		/*
		static Storage<meta::Null> *Cast(Registry &, Reference)
		{
			Storage<Parent *> *parent = NewStorage<Parent *>(R);
			**parent = &Q;
			return parent;
		}
		*/
	};

	// Assignment
	template <class, bool>
	struct AssignOp
	{
		static void Perform(Reference, ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::Assign);
		}
	};
	template <class Dummy>
	struct AssignOp<Dummy, true>
	{
		static void Perform(Reference A, ConstReference B)
		{
			A = B;
		}
	};

	// absolute
	template <class Dummy, bool>
	struct AbsoluteOp
	{
		static void Perform(Reference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::Absolute);
		}
	};

	template <class Dummy>
	struct AbsoluteOp<Dummy, true>
	{
		static void Perform(Reference value)
		{
			SetAbsoluteValue(value);
		}
	};

	template <class Dummy, bool>
	struct LessOp
	{
		static bool Perform(ConstReference, ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::Less);
		}
	};

	template <class Dummy>
	struct LessOp<Dummy, true>
	{
		static bool Perform(ConstReference A, ConstReference B)
		{
			return A < B;
		}
	};

	template <class Dummy, bool>
	struct EquivOp
	{
		static bool Perform(ConstReference A, ConstReference B)
		{
			return A == B;
		}
	};

	template <class Dummy>
	struct EquivOp<Dummy, false>
	{
		static bool Perform(ConstReference, ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::Equiv);
		}
	};
	/// Greater
	template <class Dummy, bool>
	struct GreaterOp
	{
		static bool Perform(ConstReference, ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::Greater);
		}
	};

	template <class Dummy>
	struct GreaterOp<Dummy, true>
	{
		static bool Perform(ConstReference A, ConstReference B)
		{
			return A > B;
		}
	};

	// TODO: implement arithmetic using += etc
	template <class Dummy, bool>
	struct PlusOp
	{
		static Store Perform(ConstReference, ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::Plus);
		}
	};

	template <class Dummy>
	struct PlusOp<Dummy, true>
	{
		static Store Perform(ConstReference A, ConstReference B)
		{
			return A + B;
		}
	};

	// minus
	template <class, bool>
	struct MinusOp { static Store Perform(ConstReference A, ConstReference B)
		{
			return A - B;
		}
	};

	template <class D>
	struct MinusOp<D, false>
	{
		static Store Perform(ConstReference, ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::Minus);
		}
	};

	template <class D, bool>
	struct MultiplyOp
	{
		static Store Perform(ConstReference , ConstReference )
		{
			KAI_THROW_2(NoOperation, Number, Properties::Multiply);
		}
	};
	template <class D>
	struct MultiplyOp<D, true>
	{
		static Store Perform(ConstReference A, ConstReference B)
		{
			return A * B;
		}
	};

	template <class, bool>
	struct DivideOp
	{
		static Store Perform(ConstReference , ConstReference )
		{
			KAI_THROW_2(NoOperation, Number, Properties::Divide);
		}
	};
	template <class D>
	struct DivideOp<D, true>
	{
		static Store Perform(ConstReference A, ConstReference B)
		{
			return A / B;
		}
	};

	template <class, bool>
	struct HashOp
	{
		static HashValue Calc(ConstReference A)
		{
	#pragma warning (push)
			// warning: unreachable code. NFI why...
	#pragma warning (disable:4702)
			return GetHash(A);
	#pragma warning (push)
		}
	};
	template <class D>
	struct HashOp<D, false>
	{
		static HashValue Calc(ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::CalcHashValue);
		}
	};

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

	typedef typename meta::If<HasProperty<Properties::Reflected>::Value != 0
		, ReflectedLifetimeManagement
		, UnReflectedLifetimeManagement>::Type 
		LifetimeManager;

	typedef AssignOp<D, HasProperty<Properties::Assign>::Value != 0> Assign;
	typedef AbsoluteOp<D, HasProperty<Properties::Absolute>::Value != 0> Absolute;

	typedef LessOp<D, HasProperty<Properties::Less>::Value != 0> Less;
	typedef EquivOp<D, HasProperty<Properties::Equiv>::Value != 0> Equiv;
	typedef GreaterOp<D, HasProperty<Properties::Greater>::Value != 0> Greater;

	typedef PlusOp<D, HasProperty<Properties::Plus>::Value != 0> Plus;
	typedef MinusOp<D, HasProperty<Properties::Minus>::Value != 0> Minus;
	typedef DivideOp<D, HasProperty<Properties::Divide>::Value != 0> Divide;
	typedef MultiplyOp<D, HasProperty<Properties::Multiply>::Value != 0> Multiply;

	template <bool, class Stream>
	struct StreamInsertOp
	{
		static void Insert(Stream &S, ConstReference X)
		{
			S << X;
		}
	};

	template <class Stream>
	struct StreamInsertOp<false, Stream>
	{
		static void Insert(Stream &, ConstReference)
		{
			//nKAI_THROW_1("Can't insert into stream");
		}
	};

	template <bool, class Stream>
	struct StreamExtractOp
	{
		static void Extract(Stream &S, Reference X)
		{
			S >> X;
		}
	};
	template <class Stream>
	struct StreamExtractOp<false, Stream>
	{
		static void Extract(Stream &, ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::StreamExtract);
		}
	};

	typedef StreamInsertOp<HasProperty<Properties::StringStreamInsert>::Value != 0, KAI_NAMESPACE(StringStream)> StringStreamInsert;
	typedef StreamExtractOp<HasProperty<Properties::StringStreamExtract>::Value != 0, KAI_NAMESPACE(StringStream)> StringStreamExtract;
	typedef StreamInsertOp<HasProperty<Properties::BinaryStreamInsert>::Value != 0, KAI_NAMESPACE(BinaryStream)> BinaryStreamInsert;
	typedef StreamExtractOp<HasProperty<Properties::BinaryStreamExtract>::Value != 0, KAI_NAMESPACE(BinaryPacket)> BinaryPacketExtract;
	//typedef typename StreamInsert<HasProperty<Properties::XmlStreamInsert>::Value, XmlStream> XmlStreamInsert;

	typedef HashOp<D, HasProperty<Properties::NoHashValue>::Value != 0> HashFunction;
};

KAI_TYPE_END


namespace boost
{
	/*
	inline size_t hash_value(KAI_NAMESPACE(Handle) const &H)
	{
		throw;
		//return H.GetValue();
	}

	inline size_t hash_value(::kai::Number const &H)
	{
		throw;
		//return H.GetValue();
	}
	*/
}

