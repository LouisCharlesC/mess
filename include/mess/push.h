/**
 * @file push.h
 * @author Louis-Charles Caron (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include <utility>

namespace mess
{
	template<typename Name, typename Type>
	struct Pushed
	{
		constexpr Pushed(auto&& arg):
			value(std::forward<decltype(arg)>(arg))
		{}

		Type value;
	};
} // namespace mess