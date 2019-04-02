
/**
 * @file cameratopics.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "image.h"

#include <memory>

struct CameraImageTopic
{
	using Message = std::shared_ptr<Image>;
};
