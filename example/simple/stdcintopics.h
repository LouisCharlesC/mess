/**
 * @file stdcintopics.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "messtraits.h"

struct Quit {};

namespace mess
{
	template<> struct TopicTraits<Quit> {};
} // namespace mess
