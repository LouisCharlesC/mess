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

#include <type_traits>

namespace mess
{
	namespace impl
	{
		using IsMember = ::std::true_type;
		using IsNonMember = ::std::false_type;
	} // namespace impl

	template <auto F>
	struct IsTheResultOfCalling {};
	template <typename C, typename R, typename... Args, R(C::*F)(Args...)>
	struct IsTheResultOfCalling<F>
	{
		static constexpr auto Function = F;
		using Class = C;
	};
	template <typename C, typename R, typename... Args, R(C::*F)(Args...) const>
	struct IsTheResultOfCalling<F>
	{
		static constexpr auto Function = F;
		using Class = C;
	};
	template <typename R, typename... Args, R(*F)(Args...)>
	struct IsTheResultOfCalling<F>
	{
		static constexpr auto Function = F;
		using MemberOrNot = ::mess::impl::IsNonMember;
	};

	template<typename I>
	struct OnInstance
	{
		using Instance = I;
		using MemberOrNot = ::mess::impl::IsMember;
	};

	template<typename... Args>
	struct WithArguments {};
	using WithNoArgument = WithArguments<>;
	template<typename Arg>
	using WithArgument = WithArguments<Arg>;

	template<typename> [[nodiscard]] constexpr decltype(auto) pull();

	template<typename O>
	struct Impl
	{
		template<typename... Is>
		static constexpr decltype(auto) memberOrNot(::mess::impl::IsNonMember, typename O::template WithArguments<Is...>)
		{
			return O::IsTheResultOfCalling::Function(::mess::pull<Is>()...);
		}
		template<typename... Is>
		static constexpr decltype(auto) memberOrNot(::mess::impl::IsMember, typename O::template WithArguments<Is...>)
		{
			return (pull<typename O::OnInstance::Instance>().*O::IsTheResultOfCalling::Function)(pull<Is>()...);
		}
	};

	template<typename O>
	[[nodiscard]] constexpr decltype(auto) pull()
	{
		return ::mess::Impl<O>::memberOrNot(typename O::MemberOrNot(), typename O::WithArguments());
	}
} // namespace mess