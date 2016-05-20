#pragma once

#include <KAI/Core/Object/ClassBase.h>
#include <KAI/Core/Value.h>
#include "KAI/Core/Registry.h"
#include "KAI/Language/Common/Process.h"

KAI_BEGIN

struct ProcessCommon : Process
{
	template <class T>
	Value<T> New()
	{
		return _reg.New<T>();
	}

	template <class T>
	Value<T> New(const T& val)
	{
		return _reg.New<T>(val);
	}

	ProcessCommon() : _reg(*std::shared_ptr<Registry>()) { }
	ProcessCommon(Registry &r) : _reg(r) { }

protected:
	Registry &_reg;
};

KAI_END
