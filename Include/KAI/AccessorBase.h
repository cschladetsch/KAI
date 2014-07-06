
#ifdef KAI_HAVE_PRAGMA_ONCE
#	pragma once
#endif

#ifndef KAI_ACCESSOR_BASE_H
#	define KAI_ACCESSOR_BASE_H

KAI_BEGIN

struct AccessorBase : PropertyBase
{
	AccessorBase(Label const &F, Type::Number C, Type::Number N, bool is_system, CreateParams::Params CP) : PropertyBase(F, C, N, is_system, CP) { }

	void SetValue(Object const &, Object const &) const
	{
		KAI_THROW_0(ConstError);
	}
};

struct MutatorBase : AccessorBase
{
	MutatorBase(Label const &F, Type::Number C, Type::Number N, bool is_system, CreateParams::Params CP) : AccessorBase(F, C, N, is_system, CP) { }
};

KAI_END

#endif // KAI_ACCESSOR_BASE_H

//EOF
