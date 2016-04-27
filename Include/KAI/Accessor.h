#pragma once

KAI_BEGIN

namespace property_detail
{
	/// common functionality for accessors and mutators of either system or non-system types
	/// a `non-system` type is anything that is not convertible to an Object
	/// a `system type` is an Object, a Pointer<T>, a Pointer<const T>, or a Value<T> or Value<const T>
	template <class Base, class C, class T, class F>
	struct CommonBase : Base
	{
		typedef F Field;
		Field field;
		CommonBase(Field G, Label L, bool B, CreateParams::Params create_params) : Base(L, Type::Traits<C>::Number, Type::Traits<T>::Number, B,create_params), field(G) { }
	};

	/// common functionality for accessors and mutators of non-system types.
	template <class Base, class C, class T, class F>
	struct NonsystemProperty : CommonBase<Base,C,T,F>
	{
		typedef CommonBase<Base,C,T,F> Parent;
		using typename Parent::Field;

		NonsystemProperty(Field G, Label const &L, CreateParams::Params create_params) : CommonBase<Base,C,T,F>(G, L, false, create_params) { }

		// cannot get or set the property object for non-system types
		Object GetObject(Object const &) const 
		{ 
			KAI_THROW_0(NoProperty); 
		}
		void SetObject(Object const&, Object const &) const 
		{ 
			KAI_THROW_0(NoProperty); 
		}

		// non-system types cannot be marked 
		void SetMarked(Object &, bool) { }
	};

	/// common functionality for accessors and mutators of properties which are system types.
	template <class Base, class C, class T, class F>
	struct SystemProperty : CommonBase<Base,C,T,F>
	{
		typedef CommonBase<Base,C,T,F> Parent;
		using typename Parent::Field;
		using Parent::field;

		SystemProperty(Field G, Label const &L, CreateParams::Params create_params) : CommonBase<Base,C,T,F>(G,L,true,create_params) { }

		Object GetObject(Object const &Q) const
		{
			return Parent::GetValue(Q); // the 'value' of a property is the same as its object for system types
		}
		void SetObject(Object const &Q, Object const &V) const
		{
			Object &K = Deref<C>(Q).*field;
			if (K)
			{
				K.RemovedFromContainer(Q);
			}

			if (!V.Valid())
				K = Object();
			else
				(K = V).AddedToContainer(Q);
		}
		void SetMarked(Object &Q, bool B)
		{
			MarkObjectAndChildren(Deref<C>(Q).*field, B);
		}
	};

	/// accessor to non-system properties
	template <class K, class C, bool, class T, class = T>
	struct Accessor : NonsystemProperty<AccessorBase, C, T, T (K::*)>
	{
		typedef NonsystemProperty<AccessorBase, C, T, T (K::*)> Parent;
		using typename Parent::Field;
		using Parent::field;

		Accessor(Field F, Label const &L, CreateParams::Params create_params) : NonsystemProperty<AccessorBase, C, T, T (K::*)>(F,L,create_params) { }
		Object GetValue(Object const &Q) const
		{
			return Q.New(ConstDeref<C>(Q).*field);
		}
	};

	/// accessor to system properties
	template <class K, class C, class T, class S>
	struct Accessor<K, C, true, T, S> : SystemProperty<AccessorBase, C, T, S (K::*)>
	{
		typedef SystemProperty<AccessorBase, C, T, S (K::*)> Parent;
		using typename Parent::Field;
		using Parent::field;

		Accessor(Field F, Label const &L, CreateParams::Params create_params) : SystemProperty<AccessorBase, C, T, S (K::*)>(F,L,create_params) { }
		Object GetValue(Object const &Q) const
		{
			return ConstDeref<C>(Q).*field;
		}
	};

	// mutator for non-system types
	template <class K, class C, bool, class T, class S>
	struct Mutator : NonsystemProperty<MutatorBase, K, T, T (C::*)>
	{
		typedef NonsystemProperty<MutatorBase, K, T, T (C::*)> Parent;
		using typename Parent::Field;
		using Parent::field;

		Mutator(Field F, Label const &L, CreateParams::Params create_params) 
			: NonsystemProperty<MutatorBase, K, T, T (C::*)>(F,L,create_params) 
		{ 
		}
		
		Object GetValue(Object const &Q) const
		{
			return Q.GetRegistry()->New(ConstDeref<K>(Q).*field);	// have to create a new Object to return value of non-system types
		}
		
		void SetValue(Object const &Q, Object const &V) const
		{
			C &W = Deref<K>(Q);				// get the containing structure - may be a sub-object
			T &L = W.*field;				// get the l-value to assign to
			T const &R = ConstDeref<T>(V);	// get the r-value to assign from
			L = R;							// do the assignment
		}
	};

	// mutator for system types
	template <class K, class C, class T, class S>
	struct Mutator<K,C,true,T,S> : SystemProperty<MutatorBase, K, T, S (C::*)>
	{
		typedef NonsystemProperty<MutatorBase, K, T, T (C::*)> Parent;
		typedef typename Parent::Field Field;

		Mutator(Field F, Label const &L, CreateParams::Params create_params) 
			: SystemProperty<MutatorBase, K, T, S (C::*)>(F,L,create_params) { }
		Object GetValue(Object const &Q) const
		{
			return ConstDeref<K>(Q).*field;	// just return the object for system types
		}
		void SetValue(Object const &Q, Object const &V) const
		{
			Object P = Deref<K>(Q).*field;	// get the containing object
			if (!P)
			{
				Deref<K>(Q).*field = Q.GetRegistry()->New(ConstDeref<T>(V));
				return;
			}
			P.Assign(P.GetStorageBase(), V.GetStorageBase());	// use the class system to perform the assignment
		}
	};


	/// make an accessor (read-only) property
	/// K is the type of the containing structure
	/// C is the type of the structure which defines the member. this may be the same as K.
	/// T is the type of the property
	template <class K, class C, class T>
	struct MakeAccessor : Accessor<K, C, TypeInfo<T>::IsSytem, typename TypeInfo<T>::ValueType, typename TypeInfo<T>::StorageType>
	{
		typedef Accessor<K, C, TypeInfo<T>::IsSytem, typename TypeInfo<T>::ValueType, typename TypeInfo<T>::StorageType> Parent;
		typedef typename Parent::Field Field;

		MakeAccessor(Field F, Label const &L, CreateParams::Params create_params) : Accessor<K, C, TypeInfo<T>::IsSytem, typename TypeInfo<T>::ValueType, typename TypeInfo<T>::StorageType>(F, L,create_params) { }
	};

	/// make a mutator (read-write) property
	template <class K, class C, class T>
	struct MakeMutator : Mutator<K, C, TypeInfo<T>::IsSytem, typename TypeInfo<T>::ValueType, typename TypeInfo<T>::StorageType>
	{
		typedef Mutator<K, C, TypeInfo<T>::IsSytem, typename TypeInfo<T>::ValueType, typename TypeInfo<T>::StorageType> Parent;
		typedef typename Parent::Field Field;

		MakeMutator(Field F, Label const &L, CreateParams::Params create_params) : Mutator<K, C, TypeInfo<T>::IsSytem, typename TypeInfo<T>::ValueType, typename TypeInfo<T>::StorageType>(F, L,create_params) { }
	};

} // namespace property_detail

/// make a property which can only access the given class member
template <class K, class C, class T>
PropertyBase *MakeProperty(T const (C::*F), Label const &N, CreateParams::Params create_params = CreateParams::Default)
{
	return new property_detail::MakeAccessor<K,C,T>(F, N, create_params);
}

/// make a property which can only access the given class member
template <class K, class C, class T>
PropertyBase *MakeProperty(Pointer<const T> const (C::*F), Label const &N, CreateParams::Params create_params = CreateParams::Default)
{
	return new property_detail::MakeAccessor<K,C,T>(F, N, create_params);
}

/// make a property which can access and change the given class member
template <class K, class C, class T>
PropertyBase *MakeProperty(T (C::*F), Label const &N, CreateParams::Params create_params = CreateParams::Default)
{
	return new property_detail::MakeMutator<K,C,T>(F, N, create_params);
}

KAI_END
