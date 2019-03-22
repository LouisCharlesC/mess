/**
 * @file controlmessages.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

struct QuitTopic
{
	using Message = bool;
	static constexpr bool IsCacheEnabled = false;
};