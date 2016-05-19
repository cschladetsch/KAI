#pragma once

#include <KAI/Core/Config/Base.h>
#include <KAI/Core/Type/Number.h>
#include "PropertyBase.h"
#include "Label.h"

KAI_BEGIN

class AccessorBase : public PropertyBase
{
public:
	AccessorBase(Label const &F, Type::Number C, Type::Number N, bool is_system, typename CreateParams::Params CP)
		: PropertyBase(F, C, N, is_system, CP) { }

	void SetValue(Object const &, Object const &) const
	{
		KAI_THROW_0(ConstError);
	}
};

class MutatorBase : public AccessorBase
{
public:
	MutatorBase(Label const &F, Type::Number C, Type::Number N
		, bool is_system, typename CreateParams::Params CP) 
		: AccessorBase(F, C, N, is_system, CP) { }

	Object GetValue(Object const &Q) const
	{
		if (!IsSystemType())
			KAI_THROW_0(ConstError);

		return GetPropertyObject(Q, GetFieldName());
	}
};

KAI_END

