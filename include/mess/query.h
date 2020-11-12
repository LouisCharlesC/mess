/**
 * @file query.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-11-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include "kit.h"

#include <vector>

namespace mess
{
	template<typename Current, typename Range> 
	Range select_as_predecessors(const Current& current, const Range& predecessors)
	{
		return {};
	}

	std::vector<std::vector<Invocable>> successors_for(const Invocable& current)
	{
		return {};
	}

	bool is_just_ready(const std::vector<Invocable>& successor, const std::vector<bool>& state, const Invocable& current)
	{
		return {};
	}
}