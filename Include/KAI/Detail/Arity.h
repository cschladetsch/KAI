#pragma once

#include "KAI/KAI.h"

KAI_BEGIN

namespace detail
{
	// TODO: make this redundant with CallableableBase.h
	template <int N>
	struct Add
	{
		template <class... Args>
		static void Arg(Stack &input, tuple<Args...> &args)
		{
			typedef decltype(get<N>(args)) Ty;
			Object back = input.Top();
			input.Pop();
			get<N>(args) = Deref<Ty>(back);
			Add<N-1>::Arg(input, args);
		}
	};

	template <>
	struct Add<0>
	{
		template <class...Args>
		static void Arg(Stack &input, tuple<Args...> &args)
		{
			typedef decltype(get<0>(args)) Ty;
			Object back = input.Top();
			input.Pop();
			get<0>(args) = Deref<Ty>(back);
		}
	};
} 

KAI_END


