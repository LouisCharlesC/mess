/**
 * @file kit.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-11-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

// #include "mess.h"

#include <future>
#include <vector>

namespace mess
{
	class Futures
	{
	public:
		std::vector<bool> check_and_set(Value current) {return {};}
		std::vector<std::future<Value>>& get_futures() {return m_futures;}
		std::vector<Value&> get_values() {return {};}

	private:
		std::vector<std::future<Value>> m_futures;
	};
}