/**
 * @file mess.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <tuple>

namespace mess
{
	enum class From
	{
		Constant,
		Pointer,
		NonMemberFunction,
		MemberFunction,
	};

	template<typename> constexpr decltype(auto) pull();

	template<typename... Inputs>
	struct WithArguments
	{
		template<typename O>
		static constexpr decltype(auto) pull()
		{
			if constexpr (O::IsPulledFrom::what == ::mess::From::Constant)
			{
				return O::IsPulledFrom::here;
			}
			else if constexpr (O::IsPulledFrom::what == ::mess::From::Pointer)
			{
				return *O::IsPulledFrom::here;
			}
			else if constexpr (O::IsPulledFrom::what == ::mess::From::NonMemberFunction)
			{
				return O::IsPulledFrom::here(Inputs::WithArguments::template pull<Inputs>()...);
			}
			else if constexpr (O::IsPulledFrom::what == ::mess::From::MemberFunction)
			{
				return (O::OnInstance::Instance::WithArguments::template pull<typename O::OnInstance::Instance>().*O::IsPulledFrom::here)(Inputs::WithArguments::template pull<Inputs>()...);
			}
		}
	};
	using WithNoArgument = WithArguments<>;
	template<typename Arg>
	using WithArgument = WithArguments<Arg>;

	template<auto Pointer>
	struct IsPulledFrom: WithNoArgument
	{
		static constexpr auto here = Pointer;
		static constexpr auto what = ::mess::From::Pointer;
	};
	template <typename C, typename R, typename... Args, R(C::*Function)(Args...)>
	struct IsPulledFrom<Function>
	{
		static constexpr auto here = Function;
		static constexpr auto what = ::mess::From::MemberFunction;
	};
	template <typename C, typename R, typename... Args, R(C::*Function)(Args...) const>
	struct IsPulledFrom<Function>
	{
		static constexpr auto here = Function;
		static constexpr auto what = ::mess::From::MemberFunction;
	};
	template <typename R, typename... Args, R(*Function)(Args...)>
	struct IsPulledFrom<Function>
	{
		static constexpr auto here = Function;
		static constexpr auto what = ::mess::From::NonMemberFunction;
	};

	template<typename I>
	struct OnInstance
	{
		using Instance = I;
	};

	template<auto Constant>
	struct IsTheConstant: WithNoArgument
	{
		struct IsPulledFrom
		{
			static constexpr auto here = Constant;
			static constexpr auto what = ::mess::From::Constant;
		};
	};

	template<typename O>
	constexpr decltype(auto) pull()
	{
		return O::WithArguments::template pull<O>();
	}
	template<typename... Os>
	constexpr decltype(auto) pullAsTuple()
	{
		return std::make_tuple(::mess::pull<Os>()...);
	}
} // namespace mess