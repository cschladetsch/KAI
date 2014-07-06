
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_BASE_POINTER_H
#	define KAI_BASE_POINTER_H

KAI_BEGIN

struct BasePointerBase : Reflected<BasePointerBase>
{
	void Create() {}
	bool Destroy() { return true; }

	static void Register(Registry &);
};

StringStream &operator<<(StringStream &, const BasePointerBase &);

template <class T>
struct BasePointer : BasePointerBase, FwdBasePointer<T>
{
	typedef T Base;
	typedef typename Type::Traits<Base> BaseTraits;
	boost::shared_ptr<Base> base;

	BasePointer(Base *P = 0) : base(P) { }

	Base *GetBase() const { return &*base; }
	Base *operator->() const { return GetBase(); }
	Base &operator*() const { return *GetBase(); }

	static void Register(Registry &R)
	{
		ClassBuilder<BasePointer<T> >(R, String("BasePointer"))
			;
	}
};

KAI_TYPE_TRAITS(BasePointerBase, Type::Number::BasePointer, Type::Properties::StringStreamInsert | Type::Properties::Reflected);

KAI_END

#include KAI_TEST(X)

#endif // KAI_BASE_POINTER_H

//EOF

