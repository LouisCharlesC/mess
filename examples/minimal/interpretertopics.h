/**
 * @file interpretertopics.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-04-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "mess/traits.h"

#include <string>

// Declare components
class Interpreter;

// Declare topics
struct Interpret;
struct Quit;

// Define topic traits
namespace mess
{
	template<> struct Topic<Interpret>: Service<Interpret, Interpreter, void, const std::string&> {};
	template<> struct Topic<Quit>: Notification<Quit> {};
} // mess