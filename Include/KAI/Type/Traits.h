
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_TYPE_TRAITS_H
#	define KAI_TYPE_TRAITS_H

#	include "KAI/Meta/Base.h"

KAI_BEGIN

template <class T>
void SetAbsoluteValue(T &value)
{
	if (value < 0)
		value = -value;
}

/// Used for tri-color garbage collection
struct ObjectColor
{
	enum Color
	{
		White, Grey, Black
	};
};

StringStream& operator<<(StringStream&, ObjectColor::Color);

/// Base for all fully Reflected<T> types
struct ReflectedBase
{
	StorageBase *Self;

	virtual ~ReflectedBase() { }
	
	virtual void Create() { }				// called after object constructed, but before first use
	virtual bool Destroy() { return true; }	// called when object moved to deathrow, but before deleted
	virtual void Delete() { }				// called immediately before resources are released
};

typedef nstd::size_t HashValue;

template <class T>
struct FwdBasePointer { };

template <class T>
Storage<T> *NewStorage(Registry &);

namespace Type
{
	template <class T>
	struct NoTraitsDefined;

	template <class T>
	struct Traits
	{
		typedef typename NoTraitsDefined<T>::ForType Type;
	};

	template <>
	struct Traits<None>
	{
		enum { Number = Number::None };
		enum { Properties = 0 };

		typedef None Type;
		typedef None Store;
		typedef None *Pointer;
		typedef None const *ConstPointer;
		typedef None &Reference;
		typedef None const &ConstReference;	
		static const char *Name;
	};

	template <class T>
	struct Traits<const T> : Traits<T>
	{
	};

	/// Describes the properties of a promoted type
	struct Properties
	{
		enum Type
		{
			None = 0,
			Assign = 1 << 0,
			Reflected = 1 << 1,

			Plus = 1 << 2,
			Minus = 1 << 3,
			Divide = 1 << 4,
			Multiply = 1 << 5,
			Arithmetic = Plus | Minus | Divide | Multiply,
			
			Less = 1 << 6,
			Equiv = 1 << 7,
			Greater = 1 << 8,
			Relational = Less | Equiv | Greater,
			
			StringStreamInsert = 1 << 9,
			StringStreamExtract = 1 << 10,
			BinaryStreamInsert = 1 << 11,
			BinaryStreamExtract = 1 << 12,
			BinaryStreaming = BinaryStreamInsert | BinaryStreamExtract,
			StringStreaming = StringStreamInsert | StringStreamExtract,
			Streaming = BinaryStreaming | StringStreaming,
			StreamInsert = StringStreamInsert | BinaryStreamInsert,
			StreamExtract = StringStreamExtract | BinaryStreamExtract,

			NoHashValue = 1 << 13,
			CalcHashValue = 1 << 14,

			// KAI why does this make all Reflected types also Container's????
			Container = /*Reflected | */(1 << 15),
			Process = 1 << 16,
			Absolute = 1 << 17,
		};
	};

	template <class T>
	struct StorageType
	{
		typedef T Type;
	};


		template <typename Reference, bool IsContainer = true>
		struct ContainerOperations
		{
			struct ColorSetter
			{
				ObjectColor::Color _c;

				ColorSetter(ObjectColor::Color c) : _c(c) { }
				template <class T>
				void operator()(T &obj)
				{
					obj.SetColor(_c);
				}
			};

			static void SetMarked(Reference R, bool M)
			{
				ForEach(R, SetMarked(M));
			}
			static void SetSwitch(Reference R, int S, bool M)
			{
				ForEach(R, SetSwitch(S, M));
			}
			static void SetColor(Reference R, ObjectColor::Color C)
			{
				ForEach(R, ColorSetter(C));
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

		template <typename Reference>
		struct ContainerOperations<Reference, false>
		{
			static void SetMarked(Reference, bool) { }
			static void SetSwitch(Reference, int, bool) { }
			static void SetColor(Reference, ObjectColor::Color) { }
			static void Erase(Reference, Object const &) { }
			template <class Fun>
			static Fun ForEachContained(Reference, Fun const &F) { return F; }
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
		struct UpCast<Reference, None>
		{
			static Storage<None> *Cast(Registry &, Reference)
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
			static Store Perform(C A, C B)
			{
				return A * B;
			}
		};
		template <typename Store, typename C, int N>
		struct MultiplyOp<Store, C, N, false>
		{
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

	template <class T, int N, int Ops = 0, class P = None, class St = typename StorageType<T>::Type, class Ref = T &, class ConstRef = T const &>
	struct TraitsBase
	{
		enum { Number = N };
		enum { Properties = Ops };

		typedef T Type;
		typedef P Parent;
		typedef St Store;
		typedef Store *Pointer;
		typedef Store const *ConstPointer;
		typedef Ref Reference;
		typedef ConstRef ConstReference;
		//static const char *Name;

		template <int N2>
		struct HasProperty { enum { Value = (Properties & N2) != 0 }; };

		template <int N2>
		struct HasProperties { enum { Value = (Properties & N2) == N2 }; };
		typedef ContainerOperations<Reference, HasProperty<Properties::Container>::Value> ContainerOps;

		typedef UpCast<Reference, Parent> UpCaster;
		
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
		typedef MultiplyOp<St, ConstReference, HasProperty<Properties::Multiply>::Value> Multiply;

		template <bool, class Stream>
		struct StreamInsert
		{
			static void Insert(Stream &S, ConstReference X)
			{
				S << X;
			}
		};
		template <class Stream>
		struct StreamInsert<false, Stream>
		{
			static void Insert(Stream &, ConstReference)
			{
				KAI_THROW_2(NoProperty, Number, Properties::StreamInsert);
			}
		};
		template <bool, class Stream>
		struct StreamExtract
		{
			static void Extract(Stream &S, Reference X)
			{
				S >> X;
			}
		};
		template <class Stream>
		struct StreamExtract<false, Stream>
		{
			static void Extract(Stream &, ConstReference)
			{
				KAI_THROW_2(NoProperty, Number, Properties::StreamExtract);
			}
		};
		typedef StreamInsert<HasProperty<Properties::StringStreamInsert>::Value, StringStream> StringStreamInsert;
		typedef StreamExtract<HasProperty<Properties::StringStreamExtract>::Value, StringStream> StringStreamExtract;
		typedef StreamInsert<HasProperty<Properties::BinaryStreamInsert>::Value, BinaryStream> BinaryStreamInsert;
		typedef StreamExtract<HasProperty<Properties::BinaryStreamExtract>::Value, BinaryPacket> BinaryPacketExtract;
		//typedef typename StreamInsert<HasProperty<Properties::XmlStreamInsert>::Value, XmlStream> XmlStreamInsert;

		typedef HashOp<ConstReference, Number, !HasProperty<Properties::NoHashValue>::Value> HashFunction;
	};
}

#define KAI_TYPE_TRAITS_NOASSIGN(T, N, Ops) \
	namespace Type \
	{ \
		template <> \
		struct Traits<T> : TraitsBase<T, N, Ops> { static const char *Name; }; \
		const char *Traits<T>::Name = #T; \
	}

#define KAI_TYPE_TRAITS_NAMED(T, N, M, Ops) \
	namespace Type \
	{ \
		template <> \
		struct Traits<T> : TraitsBase<T, N, Ops> { static const char *Name; }; \
		const char *Traits<T>::Name = #T; \
	}

#define KAI_TYPE_TRAITS(T, N, Ops) \
	KAI_TYPE_TRAITS_NAMED(T, N, #T, Ops | Properties::Assign)


#define KAI_TYPE_TRAITS_BASIC(T, N) \
	KAI_TYPE_TRAITS(T,N,0)

#define KAI_TYPE_TRAITS_REFLECTED(T, N) \
	KAI_TYPE_TRAITS(T,N,Type::Properties::Reflected)

#define KAI_TYPE_TRAITS_ARITHMETIC(T, N) \
	KAI_TYPE_TRAITS(T,N,Type::Properties::Arithmetic | Type::Properties::StringStreamInsert)

#define KAI_TYPE_TRAITS_BASIC_STRING_STREAM_INSERT(T,N) \
	KAI_TYPE_TRAITS(T,N,Type::Properties::StringStreamInsert)

//#define KAI_TYPE_TRAITS_DERIVED(T,N,P,R) \
//	namespace Type \
//	{ \
//		template <> \
//		struct Traits<T> : TraitsBase<T, N, P,R> { static const char *Name; }; \
//		const char *Traits<T>::Name = #T; \
//	}
//
HashValue GetHash(const String &);

KAI_TYPE_TRAITS(String, Number::String
	, Properties::Streaming 
	| Properties::Plus |
	Properties::Equiv  | Properties::Relational);

KAI_TYPE_TRAITS(StringStream, Number::StringStream, 0);

//namespace Type
//{
//	template <>
//	struct Traits<Handle> : TraitsBase<Handle, Number::Array
//		, Properties::StringStreamInsert>
//	{
//		static const char *Name;
//	};
//
//	const char * Traits<Handle>::Name = "Handle";
//}

KAI_TYPE_TRAITS(Handle, Number::Handle, Properties::StringStreamInsert);

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
	inline size_t hash_value(KAI_NAMESPACE(Handle) const &H)
	{
		return H.GetValue();
	}
	inline size_t hash_value(KAI_NAMESPACE(Type::Number) const &H)
	{
		return H.GetValue();
	}
}

#endif // KAI_TYPE_TRAITS_H

//EOF
