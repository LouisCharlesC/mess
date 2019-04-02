/**
 * @file processortopics.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-25
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "image.h"

struct ProcessorImageTopic
{
	using Message = Image;
};
struct ProcessorImageTimedOutTopic
{
	using Message = Image::FrameID;
};