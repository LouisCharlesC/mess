/**
 * @file sensor.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "state.hpp"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <thread>

class Sensor
{
public:
	~Sensor();

	void quit();

	std::chrono::milliseconds getPeriod();
	void setPeriod(std::chrono::milliseconds period);

private:
	void loop();

	std::atomic<bool> m_quit = ATOMIC_VAR_INIT(false);

	std::uint64_t m_data = 0;
	mess::State<std::chrono::milliseconds> m_period{std::chrono::milliseconds(10)};

	std::thread m_thread{&Sensor::loop, this};
};
