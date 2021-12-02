/**
 * @file pull.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-11-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include "kit.h"
#include "query.h"

#include <any>
#include <functional>
#include <future>
#include <vector>

namespace mess
{
	std::future<void> thunk(Invocable& current, std::vector<Value&> predecessors, Futures futures)
	{
		auto state = futures.check_and_set(current(predecessors)); // execute current task, send its output, check if sending this allows any successor to be run

		for (auto& successor: successors_for(current))
		{
			if (is_just_ready(successor, state, current))
			{
				std::async(scope, successor, select_as_predecessors(successor, futures.get_values()));
			}
		}
	}
	
	void scope(std::vector<Invocable> currents, std::vector<Value&> provided)
	{
		Futures futures;

		std::vector<std::future<void>> scopes(currents.size());
		for (std::size_t i=0; i < currents.size(); ++i)
		{
			scopes[i] = std::async(thunk, currents[i], select_as_predecessors(currents[i], predecessors), futures);
		}

		for (auto& scope: scopes)
		{
			scope.wait(); // this is bad, this also must be continued by the last child process
		}
	}

	template<typename... Pulled>
	auto pull()
	{

	}
}