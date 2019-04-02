/**
 * @file image.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-25
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <array>
#include <chrono>
#include <cstdint>

struct Image
{
	using FrameID = std::uint64_t;

	FrameID frameId = 0;
	std::chrono::steady_clock::time_point timestamp;
	std::array<std::uint8_t, 640*480> image;
};
