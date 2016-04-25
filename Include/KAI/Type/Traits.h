#pragma once

KAI_TYPE_BEGIN

template <typename T>
struct NoTraitsDefined;

template <typename T>
struct Traits
{
	typedef typename NoTraitsDefined<T>::ForType Type;
};

typedef int TypeNumber;

template <>
struct Traits<meta::Null>
{
	enum { Num = Number::None };
	enum { Properties = 0 };

	typedef meta::Null Type;
	typedef meta::Null Store;
	typedef meta::Null *Pointer;
	typedef meta::Null const *ConstPointer;
	typedef meta::Null &Reference;
	typedef meta::Null const &ConstReference;	
	static const char *Name;
};

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
	enum { Number = static_cast<int>(E) };
	enum { Props = static_cast<int>(Q) };
	typedef T Type;
	//typedef P Parent;
	typedef St Store;
	typedef Store *Pointer;
	typedef Store const *ConstPointer;
	typedef Ref Reference;
	typedef ConstRef ConstReference;

	//typedef UpCast<Reference, Parent> UpCaster;
	
	template <int N2>
	struct HasProperty
	{
		enum
		{
			Value = (N2 & (int)Props) != 0
		};
	};

	template <int N3>
	struct HasProperties { enum { Value = ((int)Props & N3) == N3 }; };

	template <bool>
	struct ContainerOperations
	{
		static void SetMarked(Reference R, bool M)
		{
			ForEach(R, sif::SetMarked<T>(M));
		}
		static void SetSwitch(Reference R, int S, bool M)
		{
			ForEach(R, sif::SetSwitch<T>(S, M));
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
	typedef ContainerOperations<HasProperty<Properties::Container>::Value> ContainerOps;

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
			// TODO EXCEPTION
			//			KAI_THROW_2(NoProperty, Number, Properties::StreamExtract);
			throw "No such property";
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

