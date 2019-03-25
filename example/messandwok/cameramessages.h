/**
 * @file cameramessages.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <memory>

struct Image
{
	std::uint64_t frameId = 0;
	std::chrono::steady_clock::time_point timestamp;
	std::array<std::uint8_t, 640*480> image;
};
struct CameraImageTopic
{
	using Message = std::shared_ptr<Image>;
	static constexpr bool IsCacheEnabled = false;
};
class Camera;
struct GetCameraPeriodService
{
	using ReturnType = std::chrono::milliseconds;
	using Provider = Camera;
};