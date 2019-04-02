/**
 * @file systemtopics.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "messmacros.h"

enum class SystemFSM
{
	Init,
	Start,
	Quit
};

MESS_DEFINE_PUBLICATION(SystemFSMTopic, SystemFSM)