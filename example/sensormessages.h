/**
 * @file sensormessages.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <cstdint>

struct SensorTopic
{
	using Message = std::uint64_t;
	static constexpr bool IsCacheEnabled = true;
};