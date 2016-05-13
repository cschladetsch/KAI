#pragma once

KAI_BEGIN

namespace detail
{
	using namespace std;
	using namespace meta;

	template <typename F, typename Tuple, size_t... I>
	decltype(auto) CallFunImpl(F f, Tuple&& t, index_sequence<I...>)
	{
		return forward<F>(f)(get<I>(forward<Tuple>(t))...);
	}

	// invoke a function with any anity given arguments in a tuple
	template <typename F, typename Tuple>
	decltype(auto) CallFun(F f, Tuple&& t)
	{
		using idx = make_index_sequence<tuple_size<decay_t<Tuple>>::value>;
		return CallFunImpl(forward<F>(f), forward<Tuple>(t), idx{});
	}

	template <class Q, typename F, typename Tuple, size_t... I>
	decltype(auto) CallMethodImpl(Q &q, F f, Tuple&& t, index_sequence<I...>)
	{
		auto m = forward<F>(f);
		return (q.*m)(get<I>(forward<Tuple>(t))...);
	}

	// invoke a method of any arity given arguments in a tuple
	template <class Q, typename F, typename Tuple>
	decltype(auto) CallMethod(Q &q, F f, Tuple&& t)
	{
		using idx = make_index_sequence<tuple_size<decay_t<Tuple>>::value>;
		return CallMethodImpl(q, forward<F>(f), forward<Tuple>(t), idx{});
	}

	// pull arguments into a tuple, recursively
	template <int N>
	struct Add
	{
		template <class... Args>
		static void Arg(Stack &input, tuple<Args...> &args)
		{
			typedef decltype(std::get<N>(args)) Ty;
			Object back = input.Top();
			input.Pop();
			get<N>(args) = Deref<Ty>(back);
			Add<N-1>::Arg(input, args);
		}
	};

	// pull last argument
	template <>
	struct Add<0>
	{
		template <class...Args>
		static void Arg(Stack &input, tuple<Args...> &args)
		{
			typedef decltype(std::get<0>(args)) Ty;
			Object back = input.Top();
			input.Pop();
			get<0>(args) = Deref<Ty>(back);
		}
	};
} 

KAI_END

