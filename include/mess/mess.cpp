/**
 * @file mess.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-10-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <cassert>
#include <iostream>
#include <string>
#include <tuple>


static bool gt(int lhs, int rhs)
{
	return lhs>rhs;
}
static int sum(int lhs, int rhs)
{
	return lhs+rhs;
}

namespace mess
{
	template<typename... Tags>
	auto resolve(Tags... tags)
	{
		return std::make_tuple(tags.resolve(tags)...);
	}
	int resolve(int i)
	{
		return i;
	}

	template<typename Invocable>
	struct Invokes
	{
		Invokes(Invocable& invocable):
			m_invocable(invocable)
		{}

		Invocable m_invocable;
	};

	namespace ct
	{
		template<auto Invocable>
		struct Invokes
		{
			static constexpr auto invocable = Invocable;
		};
	}
	
	template<typename... Tags>
	struct WithArguments
	{
		WithArguments(const Tags&... tags):
			m_tags(tags...)
		{}

		template<typename Invocable>
		auto resolve(const Invokes<Invocable>& self) const
		{
			return self.m_invocable(mess::resolve(std::get<Tags>(m_tags))...);
		}

		const std::tuple<const Tags&...> m_tags;
	};
	template<typename... Mixins>
	struct Name: Mixins...
	{
		constexpr Name(Mixins... mixins):
			Mixins(mixins)...
		{}

		template<typename... Tags>
		Name<WithArguments<Tags...>, Mixins...> withArguments(const Tags&... tags)
		{
			return {{tags...}, static_cast<Mixins&>(*this)...};
		}
		Name<WithArguments<>, Mixins...> withNoArgument()
		{
			return {{}, static_cast<Mixins&>(*this)...};
		}

		template<typename Invocable>
		auto invokes(Invocable invocable)
		{
			return Name<Invokes<Invocable>, Mixins...>{{invocable}, static_cast<Mixins&>(*this)...};
		}
	};
}

// is, invokes, dereferences, replaces
// from_instance
// with_arguments, with_argument, with_no_argument
// when, when_all, when_any
// executed_by, executed_inline
// finish

int main()
{
	mess::Name input; // these must be finished names
	mess::Name state;

	auto update = mess::Name().invokes([](auto&& lhs, auto&& rhs){return sum(lhs, rhs);}).withArguments(input, state);
	auto output = mess::Name().invokes(&gt).withArguments(input, update);
	std::cout << sizeof(output);

	int in = 1;
	int updated = 0;
	int out = 0;




	



	// auto& [out, updated] = resolve(output, update).provided(in);

	// assert( == 2);
	// assert(resolve(second) == 42);
	// static_assert(resolve(first) == 2, "");
	// static_assert(resolve(second) == 42, "");

	return 0;
}