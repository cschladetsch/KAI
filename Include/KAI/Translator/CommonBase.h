#pragma once

#include "KAI/KAI.h"
#include "KAI/Translator/Process.h"

KAI_BEGIN

struct CommonBase : Process
{
	template <class T>
	Value<T> New()
	{
		return reg.New<T>();
	}

	template <class T>
	Value<T> New(const T& val)
	{
		return reg.New<T>(val);
	}

	CommonBase() : reg(*std::shared_ptr<Registry>()) { }
	CommonBase(Registry &r) : reg(r) { }

protected:
	Registry &reg;
};

KAI_END


