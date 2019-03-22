/**
 * @file stdcinmessages.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <string>

struct StdcinTopic
{
	using Message = std::string;
	static constexpr bool IsCacheEnabled = false;
};