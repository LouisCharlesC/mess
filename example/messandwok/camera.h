/**
 * @file camera.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "broker.h"
#include "cameratopics.h"
#include "systemtopics.h"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <numeric>
#include <thread>

class Camera
{
public:
	Camera(std::chrono::milliseconds period):
		m_period(period)
	{}

	~Camera()
	{
		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}

	void onPublish(SystemFSMTopic, const SystemFSMTopic::Message& state)
	{
		if (state == SystemFSM::Start)
		{
			m_thread = std::thread(&Camera::loop, this);
		}
		else if (state == SystemFSM::Quit)
		{
			assert(m_thread.joinable());
			m_quit = true;
			m_thread.join();
		}
	}

	void loop()
	{
		auto nextTrigger = std::chrono::high_resolution_clock::now();
		while(!m_quit)
    {
				CameraImageTopic::Message image = std::make_shared<CameraImageTopic::Message::element_type>();
				std::iota(image->image.begin(), image->image.end(), m_frameId);
				image->frameId = m_frameId;
				++m_frameId;
				image->timestamp = std::chrono::steady_clock::now();

				nextTrigger += m_period;
				std::this_thread::sleep_until(nextTrigger);
				Broker::publish<CameraImageTopic>(image);
		}
	}

private:
	std::atomic<bool> m_quit = ATOMIC_VAR_INIT(false);
	std::chrono::milliseconds m_period;
	std::uint64_t m_frameId = 0;
	std::thread m_thread;
};