/**
 * @file interpretersubs.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-04-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "interpretertopics.h"

// Declare components
class Console;

// Declare callback
namespace mess
{
	template<> template<>
	void Topic<Quit>::Notification::onNotify(Console& subscriber);
} // mess