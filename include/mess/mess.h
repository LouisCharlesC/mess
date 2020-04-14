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
	using IsMember = std::true_type;
	using IsNonMember = std::false_type;

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
		using MemberOrNot = IsNonMember;
	};

	template<typename I>
	struct OnInstance
	{
		using Instance = I;
		using MemberOrNot = IsMember;
	};

	template<typename... Is>
	struct WithArguments {};
	using WithNoArgumentss = WithArguments<>;

	struct Frame
	{
		template<typename O>
		[[nodiscard]] static constexpr decltype(auto) pull()
		{
			return memberOrNot<O>(typename O::MemberOrNot(), typename O::WithArguments());
		}

	private:
		template<typename O, typename... Is>
		static constexpr decltype(auto) memberOrNot(IsNonMember, typename O::template WithArguments<Is...>)
		{
			return O::Call::Function(pull<Is>()...);
		}
		template<typename O, typename... Is>
		static constexpr decltype(auto) memberOrNot(IsMember, typename O::template WithArguments<Is...>)
		{
			return (pull<typename O::OnInstance::Instance>().*O::Call::Function)(pull<Is>()...);
		}
	};

} // namespace mess