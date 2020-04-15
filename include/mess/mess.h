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
#include <type_traits>

namespace mess
{
	namespace impl
	{
		struct MemberFunction {};
		struct NonMemberFunction {};
		struct Pointer {};
		struct Constant {};
	} // namespace impl

	template<typename... Args>
	struct WithArguments {};
	using WithNoArgument = WithArguments<>;
	template<typename Arg>
	using WithArgument = WithArguments<Arg>;

	template<auto P>
	struct IsPulledFrom: WithNoArgument
	{
		static constexpr auto pointer = P;
		using Nature = ::mess::impl::Pointer;
	};
	template <typename C, typename R, typename... Args, R(C::*F)(Args...)>
	struct IsPulledFrom<F>
	{
		static constexpr auto Function = F;
		using Class = C;
	};
	template <typename C, typename R, typename... Args, R(C::*F)(Args...) const>
	struct IsPulledFrom<F>
	{
		static constexpr auto Function = F;
		using Class = C;
	};
	template <typename R, typename... Args, R(*F)(Args...)>
	struct IsPulledFrom<F>
	{
		static constexpr auto Function = F;
		using Nature = ::mess::impl::NonMemberFunction;
	};

	template<typename I>
	struct OnInstance
	{
		using Instance = I;
		using Nature = ::mess::impl::MemberFunction;
	};

	template<auto C>
	struct IsTheConstant: WithNoArgument
	{
		static constexpr auto constant = C;
		using Nature = ::mess::impl::Constant;
	};

	template<typename... Vs>
	struct PushesTo {};

	template<typename> constexpr decltype(auto) pull();

	template<typename O>
	struct Impl
	{
		template<typename... Is>
		static constexpr decltype(auto) nature(::mess::impl::NonMemberFunction, typename O::template WithArguments<Is...>)
		{
			return O::IsPulledFrom::Function(::mess::pull<Is>()...);
		}
		template<typename... Is>
		static constexpr decltype(auto) nature(::mess::impl::MemberFunction, typename O::template WithArguments<Is...>)
		{
			return (pull<typename O::OnInstance::Instance>().*O::IsPulledFrom::Function)(pull<Is>()...);
		}
		static constexpr decltype(auto) nature(::mess::impl::Constant, typename O::template WithArguments<>)
		{
			return O::IsTheConstant::constant;
		}
		static constexpr decltype(auto) nature(::mess::impl::Pointer, typename O::template WithArguments<>)
		{
			return *O::IsPulledFrom::pointer;
		}
	};

	template<typename O>
	constexpr decltype(auto) pull()
	{
		return ::mess::Impl<O>::nature(typename O::Nature(), typename O::WithArguments());
	}
	template<typename O1, typename O2, typename... Os>
	constexpr decltype(auto) pull()
	{
		return std::make_tuple(
			::mess::Impl<O1>::nature(typename O1::Nature(), typename O1::WithArguments()),
			::mess::Impl<O2>::nature(typename O2::Nature(), typename O2::WithArguments()),
			::mess::Impl<Os>::nature(typename Os::Nature(), typename Os::WithArguments())...);
	}
} // namespace mess