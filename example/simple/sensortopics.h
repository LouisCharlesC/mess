/**
 * @file sensortopics.h
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

#include <chrono>
#include <cstdint>

struct Sensor;

struct SensorValueCache {};
struct GetPeriod;
struct SetPeriod;

namespace mess
{
	template<> struct TopicTraits<SensorValueCache>
	{
		using Message = std::uint64_t;
		static constexpr bool IsCacheEnabled = true;
		static const Message InitialValue = 0;
	};

	template<> struct TopicTraits<GetPeriod>
	{
		using Provider = Sensor;
		using Return = std::chrono::milliseconds;
	};
	template<> Return<GetPeriod> onCall<GetPeriod>(Provider<GetPeriod>&);

	template<> struct TopicTraits<SetPeriod>
	{
		using Provider = Sensor;
		using Return = void;
	};
	template<> Return<SetPeriod> onCall<SetPeriod>(Provider<GetPeriod>&, std::chrono::milliseconds);
}