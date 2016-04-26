#	include "KAI/Meta/Base.h"

KAI_BEGIN

// Used for tri-color garbage collection
struct ObjectColor
{
	enum Color
	{
		White, Grey, Black
	};
};

/// Base for all fully Reflected<T> types
struct ReflectedBase
{
	StorageBase *Self;

	virtual ~ReflectedBase() { }
	
	virtual void Create() { }			//< called after object constructed, but before first use
	virtual void Destroy() { }			//< called when object moved to deathrow, but before deleted
	virtual void Delete() { }			//< called immediately before resources are released
};

typedef std::size_t HashValue;

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

			Container = /*Reflected |*/ (1 << 15),
			Process = 1 << 16,
			TestOutput = 1 << 17,
		};
	};

	template <class T>
	struct StorageType
	{
		typedef T Type;
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
		static const char *Name;

		template <int N>
		struct HasProperty { enum { Value = (Properties & N) != 0 }; };
		template <int N>
		struct HasProperties { enum { Value = (Properties & N) == N }; };

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

		template <class>
		struct UpCast
		{
			static Storage<Parent *> *Cast(Registry &R, Reference Q)
			{
				Storage<Parent *> *parent = NewStorage<Parent *>(R);
				**parent = &Q;
				return parent;
			}
		};
		template <>
		struct UpCast<None>
		{
			static Storage<None> *Cast(Registry &, Reference)
			{
				return 0;
			}
		};
		typedef UpCast<Parent> UpCaster;
		
		struct UnReflectedLifetimeManagement
		{
			static void Create(Storage<T> *P)
			{
			}
			static void Destroy(Storage<T> *P)
			{
			}
			static void Delete(Storage<T> *P)
			{
				delete P;
			}
		};
		struct ReflectedLifetimeManagement
		{
			static void Create(Storage<T> *storage)
			{
				UnReflectedLifetimeManagement::Create(storage);
				ReflectedBase *reflected = &storage->GetCleanReference();
	#	ifdef KAI_DEBUG
				//TODO
				//reflected->This = &storage->GetCleanReference();
	#	endif
				reflected->Self = storage;
				reflected->Create();
			}
			static void Destroy(Storage<T> *storage)
			{
				ReflectedBase *reflected = &storage->GetReference();
				reflected->Destroy();
				UnReflectedLifetimeManagement::Destroy(storage);
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

		// Assignment
		template <bool>
		struct AssignOp
		{
			static void Perform(Reference A, ConstReference B)
			{
				A = B;
			}
		};
		template <>
		struct AssignOp<false>
		{
			static void Perform(Reference A, ConstReference B)
			{
				KAI_THROW_2(NoProperty, Number, Properties::Assign);
			}
		};
		typedef AssignOp<HasProperty<Properties::Assign>::Value> Assign;

		// Less
		template <bool>
		struct LessOp
		{
			static bool Perform(ConstReference A, ConstReference B)
			{
				return A < B;
			}
		};
		template <>
		struct LessOp<false>
		{
			static bool Perform(ConstReference A, ConstReference B)
			{
				KAI_THROW_2(NoProperty, Number, Properties::Less);
			}
		};
		/// Equiv
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
			static bool Perform(ConstReference A, ConstReference B)
			{
				KAI_THROW_2(NoProperty, Number, Properties::Equiv);
			}
		};
		/// Greater
		template <bool>
		struct GreaterOp
		{
			static bool Perform(ConstReference A, ConstReference B)
			{
				return A > B;
			}
		};
		template <>
		struct GreaterOp<false>
		{
			static bool Perform(ConstReference A, ConstReference B)
			{
				KAI_THROW_2(NoProperty, Number, Properties::Greater);
			}
		};
		typedef typename LessOp<HasProperty<Properties::Less>::Value> Less;
		typedef typename EquivOp<HasProperty<Properties::Equiv>::Value> Equiv;
		typedef typename GreaterOp<HasProperty<Properties::Greater>::Value> Greater;

		// TODO: implement arithmetic using += etc
		template <bool>
		struct PlusOp
		{
			static Store Perform(ConstReference A, ConstReference B)
			{
				return A + B;
			}
		};
		template <bool>
		struct PlusOp<false>
		{
			static Store Perform(ConstReference A, ConstReference B)
			{
				return A - B;
			}
		};
		template <bool>
		struct MinusOp<false
		{
			static Store Perform(ConstReference A, ConstReference B)
			{
				KAI_THROW_2(NoProperty, Number, Properties::Plus);
			}
		};
		template <>
		struct MinusOp<false>
		{
			static Store Perform(ConstReference A, ConstReference B)
			{
				KAI_THROW_2(NoProperty, Number, Properties::Minus);
			}
		};
		template <bool>
		struct MultiplyOp
		{
			static Store Perform(ConstReference A, ConstReference B)
			{
				return A * B;
			}
		};
		template <>
		struct MultiplyOp<false>
		{
			static Store Perform(ConstReference A, ConstReference B)
			{
				KAI_THROW_2(NoProperty, Number, Properties::Multiply);
			}
		};
		template <bool>
		struct DivideOp
		{
			static Store Perform(ConstReference A, ConstReference B)
			{
				return A / B;
			}
		};
		template <>
		struct DivideOp<false>
		{
			static Store Perform(ConstReference A, ConstReference B)
			{
				KAI_THROW_2(NoProperty, Number, Properties::Divide);
			}
		};
		typedef typename PlusOp<HasProperty<Properties::Plus>::Value> Plus;
		typedef typename MinusOp<HasProperty<Properties::Minus>::Value> Minus;
		typedef typename DivideOp<HasProperty<Properties::Divide>::Value> Divide;
		typedef typename MultiplyOp<HasProperty<Properties::Multiply>::Value> Multiply;

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
			static void Insert(Stream &S, ConstReference X)
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
			static void Extract(Stream &S, ConstReference X)
			{
				KAI_THROW_2(NoProperty, Number, Properties::StreamExtract);
			}
		};
		typedef typename StreamInsert<HasProperty<Properties::StringStreamInsert>::Value, StringStream> StringStreamInsert;
		typedef typename StreamExtract<HasProperty<Properties::StringStreamExtract>::Value, StringStream> StringStreamExtract;
		typedef typename StreamInsert<HasProperty<Properties::BinaryStreamInsert>::Value, BinaryStream> BinaryStreamInsert;
		typedef typename StreamExtract<HasProperty<Properties::BinaryStreamExtract>::Value, BinaryPacket> BinaryPacketExtract;
		//typedef typename StreamInsert<HasProperty<Properties::XmlStreamInsert>::Value, XmlStream> XmlStreamInsert;

		template <bool>
		struct HashOp
		{
			static HashValue Calc(ConstReference A)
			{
				return GetHash(A);
			}
		};
		template <>
		struct HashOp<false>
		{
			static HashValue Calc(ConstReference A)
			{
				KAI_THROW_2(NoProperty, Number, Properties::CalcHashValue);
			}
		};
		typedef HashOp<!HasProperty<Properties::NoHashValue>::Value> HashFunction;
	};
}

#define KAI_TYPE_TRAITS_NOASSIGN(T, N, Ops) \
	namespace Type \
	{ \
		template <> \
		struct Traits<T> : TraitsBase<T, N, Ops> { }; \
		const char *Traits<T>::Name = #T; \
	}

#define KAI_TYPE_TRAITS_NAMED(T, N, M, Ops) \
	namespace Type \
	{ \
		template <> \
		struct Traits<T> : TraitsBase<T, N, Ops> { }; \
		template <> \
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

#define KAI_TYPE_TRAITS_DERIVED(T,N,P,R) \
	namespace Type \
	{ \
		template <> \
		struct Traits<T> : TraitsBase<T, N,#T, P,R> { static const char *Name; }; \
		const char *Traits<T>::Name = #T; \
	}

class String;

HashValue GetHash(const String &);

KAI_END

//#endif // KAI_TYPE_TRAITS_H

//EOF