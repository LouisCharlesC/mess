/**
 * @file pushpull.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

// #include <mess/ops.h>

#include <functional>
#include <utility>

namespace mess
{
	template<typename Name, typename Type>
	struct Provided
	{
		template<typename Arg>
		constexpr Pushed(Arg&& arg):
			value(std::forward<Arg>(arg))
		{}

		Type value;
	};

	template<typename Name, typename Type>
	constexpr auto provide(const std::reference_wrapper<Type>& ref)
	{
		return Provided<Name, Type&>(ref.get());
	}
	template<typename Name, typename Type>
	constexpr auto provide(Type&& value)
	{
		return Provided<Name, std::decay_t<Type>>(std::forward<Type>(value));
	}

	// template<typename NameToPull, typename... PushedTypes, typename... PushedNames>
	// constexpr auto pull(Pushed<PushedTypes, PushedNames>&&... pushedValues)
	// {
	// 	if constexpr (NameToPull::op == Operation::Get)
	// 	{
	// 		return NameToPull::ValueToRead;
	// 	}
	// 	if constexpr (NameToPull::op == Operation::Dereference)
	// 	{
	// 		return *pull<typename NameToPull::NameToDereference>();
	// 	}
	// 	if constexpr (NameToPull::op == Operation::Invoke)
	// 	{
	// 		return pull<typename NameToPull::NameToInvoke>()();
	// 	}
	// }
} // namespace mess