/**
 * @file stdcinsubs.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "stdcintopics.h"

struct Sensor;
struct Stdcin;

namespace mess
{
	template<> void onNotify<Quit, Sensor>(Sensor&);
	template<> void onNotify<Quit, Stdcin>(Stdcin&);
} // mess
