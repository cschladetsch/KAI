#pragma once

KAI_TYPE_BEGIN

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

KAI_TYPE_END

