/**
 * @file systemmessages.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

enum class SystemFSM
{
	Init,
	Start,
	Quit
};

struct SystemFSMTopic
{
	using Message = SystemFSM;
	static constexpr bool IsCacheEnabled = false;
};