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

namespace mess
{
	template<typename O>
	struct ToProduce {};

	template <auto F>
	struct Call;

	template<typename... Is>
	struct WithArgs {};
	using WithNoArgs = WithArgs<>;

	template <typename OwnerT, typename R, typename... Args, R(OwnerT::*F)(Args...)>
	struct Call<F>
	{
		static constexpr auto func = F;
		using Return = R;
	};
	template <typename OwnerT, typename R, typename... Args, R(OwnerT::*F)(Args...) const>
	struct Call<F>
	{
		static constexpr auto func = F;
		using Return = R;
	};
	template <typename R, typename... Args, R(*F)(Args...)>
	struct Call<F>
	{
		static constexpr auto func = F;
		using Return = R;
	};

	template<template<typename> class DataFlow = ToProduce>
	struct Frame
	{
		template<typename O>
		static constexpr typename DataFlow<O>::Call::Return pull()
		{
			return DataFlow<O>::Call::func();
		}
	};

} // namespace mess