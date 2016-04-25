#pragma once

KAI_BEGIN

struct AccessorBase : PropertyBase
{
	AccessorBase(Label const &F, Type::NumberEnum C, Type::NumberEnum N, bool is_system, typename CreateParams::Params CP)
		: PropertyBase(F, C, N, is_system, CP) { }

	void SetValue(Object const &, Object const &) const
	{
		KAI_THROW_0(ConstError);
	}
};

struct MutatorBase : AccessorBase
{
	MutatorBase(Label const &F, Type::NumberEnum C, Type::NumberEnum N
		, bool is_system, typename CreateParams::Params CP) 
		: AccessorBase(F, C, N, is_system, CP) { }
};

KAI_END
