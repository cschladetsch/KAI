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

//template <class T>
//struct StorageType<FwdBasePointer<T> >
//{
//	typedef T Type;
//};

template <class T, int E, int Q, class St = typename StorageType<T>::Type, class Ref = T &, class ConstRef = T const &>
struct TraitsBase
{
	enum { Number = (E) };
	enum { Props = (Q) };
	typedef T Type;
	//typedef P Parent;
	typedef St Store;
	typedef Store *Pointer;
	typedef Store const *ConstPointer;
	typedef Ref Reference;
	typedef ConstRef ConstReference;

	//static const char *_name = 0;

	static const char *Name()
	{
		//if (_name != nullptr)
		//	return _name;

		return boost::typeindex::type_id<T>().pretty_name().c_str();
	}

	template <int N2>
	struct HasProperty { enum { Value = (N2 & Q) != 0 }; };

	template <int N3>
	struct HasProperties { enum { Value = (N3 & Q) == N3 }; };

	template <bool>
	struct UpCast
	{
		//static Storage<Parent *> *Cast(Registry &R, Reference Q)
		//{
		//	return 0;
		//}
	};

	template <>
	struct UpCast <true>
	{
		static Storage<meta::Null> *Cast(Registry &, Reference)
		{
			Storage<Parent *> *parent = NewStorage<Parent *>(R);
			**parent = &Q;
			return parent;
		}
	};

	// Assignment
	template <bool>
	struct AssignOp
	{
		static void Perform(Reference, ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::Assign);
		}
	};
	template <>
	struct AssignOp<true>
	{
		static void Perform(Reference A, ConstReference B)
		{
			A = B;
		}
	};

	// absolute
	template <bool>
	struct AbsoluteOp
	{
		static void Perform(Reference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::Absolute);
		}
	};

	template <>
	struct AbsoluteOp<true>
	{
		static void Perform(Reference value)
		{
			SetAbsoluteValue(value);
		}
	};

	template <bool>
	struct LessOp
	{
		static bool Perform(ConstReference, ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::Less);
		}
	};

	template <>
	struct LessOp<true>
	{
		static bool Perform(ConstReference A, ConstReference B)
		{
			return A < B;
		}
	};

	template <bool>
	struct EquivOp
	{
		static bool Perform(ConstReference A, ConstReference B)
		{
			return A == B;
		}
	};

	template <>
	struct EquivOp<false>
	{
		static bool Perform(ConstReference, ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::Equiv);
		}
	};
	/// Greater
	template <bool>
	struct GreaterOp
	{
		static bool Perform(ConstReference, ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::Greater);
		}
	};
	template <>
	struct GreaterOp<true>
	{
		static bool Perform(ConstReference A, ConstReference B)
		{
			return A > B;
		}
	};

	// TODO: implement arithmetic using += etc
	template <bool>
	struct PlusOp
	{
		static Store Perform(ConstReference, ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::Plus);
		}
	};

	template <>
	struct PlusOp<true>
	{
		static Store Perform(ConstReference A, ConstReference B)
		{
			return A + B;
		}
	};

	// minus
	template <bool>
	struct MinusOp { static Store Perform(ConstReference A, ConstReference B)
		{
			return A - B;
		}
	};

	template <>
	struct MinusOp<false>
	{
		static Store Perform(ConstReference, ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::Minus);
		}
	};

	template <bool>
	struct MultiplyOp
	{
		static Store Perform(ConstReference , ConstReference )
		{
			KAI_THROW_2(NoOperation, Number, Properties::Multiply);
		}
	};
	template <>
	struct MultiplyOp<true>
	{
		static Store Perform(ConstReference, ConstReference)
		{
			return A * B;
		}
	};

	template <bool>
	struct DivideOp
	{
		static Store Perform(ConstReference , ConstReference )
		{
			KAI_THROW_2(NoOperation, Number, Properties::Divide);
		}
	};
	template <>
	struct DivideOp<true>
	{
		static Store Perform(ConstReference A, ConstReference B)
		{
			return A / B;
		}
	};

	template <bool>
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
	template <>
	struct HashOp<false>
	{
		static HashValue Calc(ConstReference)
		{
			KAI_THROW_2(NoOperation, Number, Properties::CalcHashValue);
		}
	};

	template <bool>
	struct ContainerOperations
	{
		static void SetMarked(Reference R, bool M)
		{
			ForEach(R, KAI_NAMESPACE(SetMarked<T>)(M));
		}
		static void SetSwitch(Reference R, int S, bool M)
		{
			ForEach(R, KAI_NAMESPACE(SetSwitch<T>)(S, M));
		}
		static void SetColor(Reference R, ObjectColor::Color C)
		{
			ForEach(R, KAI_NAMESPACE(ColorSetter<T>(C)));
		}
		static void Erase(Reference R, Object const &Q)
		{
			R.Erase(Q);
		}
		template <class Fun>
		static Fun ForEachContained(Reference R, Fun F)
		{
			return ForEach(R, F);
		}
	};
	template <>
	struct ContainerOperations<false>
	{
		static void SetMarked(Reference, bool) { }
		static void SetSwitch(Reference, int, bool) { }
		static void SetColor(Reference, ObjectColor::Color) { }
		static void Erase(Reference, Object const &) { }
		template <class Fun>
		static Fun ForEachContained(Reference, Fun const &F) { return F; }
	};
	typedef ContainerOperations<HasProperty<Properties::Container>::Value != 0> ContainerOps;

	//template <class>
	//struct UpCast
	//{
	//	static Storage<Parent *> *Cast(Registry &R, Reference Q)
	//	{
	//		Storage<Parent *> *parent = NewStorage<Parent *>(R);
	//		**parent = &Q;
	//		return parent;
	//	}
	//};
	//template <>
	//struct UpCast<None>
	//{
	//	static Storage<None> *Cast(Registry &, Reference)
	//	{
	//		return 0;
	//	}
	//};
	//typedef UpCast<Parent> UpCaster;
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

	typedef AssignOp<HasProperty<Properties::Assign>::Value != 0> Assign;
	typedef AbsoluteOp<HasProperty<Properties::Absolute>::Value != 0> Absolute;

	typedef LessOp<HasProperty<Properties::Less>::Value != 0> Less;
	typedef EquivOp<HasProperty<Properties::Equiv>::Value != 0> Equiv;
	typedef GreaterOp<HasProperty<Properties::Greater>::Value != 0> Greater;

	typedef PlusOp<HasProperty<Properties::Plus>::Value != 0> Plus;
	typedef MinusOp<HasProperty<Properties::Minus>::Value != 0> Minus;
	typedef DivideOp<HasProperty<Properties::Divide>::Value != 0> Divide;
	typedef MultiplyOp<HasProperty<Properties::Multiply>::Value != 0> Multiply;

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

	typedef StreamInsertOp<HasProperty<Properties::StringStreamInsert>::Value != 0, ::kai::StringStream> StringStreamInsert;
	typedef StreamExtractOp<HasProperty<Properties::StringStreamExtract>::Value != 0, ::kai::StringStream> StringStreamExtract;
	typedef StreamInsertOp<HasProperty<Properties::BinaryStreamInsert>::Value != 0, ::kai::BinaryStream> BinaryStreamInsert;
	typedef StreamExtractOp<HasProperty<Properties::BinaryStreamExtract>::Value != 0, ::kai::BinaryPacket> BinaryPacketExtract;
	//typedef typename StreamInsert<HasProperty<Properties::XmlStreamInsert>::Value, XmlStream> XmlStreamInsert;

	typedef HashOp<HasProperty<Properties::NoHashValue>::Value != 0> HashFunction;
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

