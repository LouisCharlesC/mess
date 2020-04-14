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
		using IsMember = std::true_type;
		using IsNonMember = std::false_type;
	} // namespace impl

	template <auto F>
	struct Call {};
	template <typename C, typename R, typename... Args, R(C::*F)(Args...)>
	struct Call<F>
	{
		static constexpr auto Function = F;
		using Class = C;
	};
	template <typename C, typename R, typename... Args, R(C::*F)(Args...) const>
	struct Call<F>
	{
		static constexpr auto Function = F;
		using Class = C;
	};
	template <typename R, typename... Args, R(*F)(Args...)>
	struct Call<F>
	{
		static constexpr auto Function = F;
		using MemberOrNot = impl::IsNonMember;
	};

	template<typename I>
	struct OnInstance
	{
		using Instance = I;
		using MemberOrNot = impl::IsMember;
	};

	template<typename... Is>
	struct WithArguments {};
	using WithNoArgumentss = WithArguments<>;

	template<typename> [[nodiscard]] static constexpr decltype(auto) pull();

	namespace impl
	{
		template<typename O, typename... Is>
		static constexpr decltype(auto) memberOrNot(impl::IsNonMember, typename O::template WithArguments<Is...>)
		{
			return O::Call::Function(::mess::pull<Is>()...);
		}
		template<typename O, typename... Is>
		static constexpr decltype(auto) memberOrNot(impl::IsMember, typename O::template WithArguments<Is...>)
		{
			return (pull<typename O::OnInstance::Instance>().*O::Call::Function)(pull<Is>()...);
		}
	} // namespace impl

	template<typename O>
	[[nodiscard]] static constexpr decltype(auto) pull()
	{
		return ::mess::impl::memberOrNot<O>(typename O::MemberOrNot(), typename O::WithArguments());
	}
} // namespace mess