#pragma once

KAI_BEGIN

struct BasePointerBase //: Reflected
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
	std::shared_ptr<Base> base;

	BasePointer(Base *P = 0) : base(P) { }

	Base *GetBase() const { return &*base; }
	Base *operator->() const { return GetBase(); }
	Base &operator*() const { return *GetBase(); }

	static void Register(Registry &R)
	{
		KAI_UNUSED(R);

		/* MUSTFIX
		ClassBuilder<BasePointer<T> >(R, String("BasePointer"))
			;
			*/
	}
};

KAI_TYPE_TRAITS(BasePointerBase, Number::BasePointer
	, Properties::StringStreamInsert
	);

KAI_END
