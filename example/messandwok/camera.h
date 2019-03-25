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

#include "broker.h"
#include "cameramessages.h"
#include "subscribemacros.h"
#include "systemmessages.h"

#include <atomic>
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

	void onPublish(const SystemFSMTopic::Message& state, SystemFSMTopic)
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

	GetCameraPeriodService::ReturnType onCall(GetCameraPeriodService)
	{
		return m_period;
	}

	void loop()
	{
		auto nextTrigger = std::chrono::high_resolution_clock::now();
		while(!m_quit)
    {
				std::this_thread::sleep_until(nextTrigger);
				nextTrigger += m_period;

				CameraImageTopic::Message image = std::make_shared<CameraImageTopic::Message::element_type>();
				std::iota(image->image.begin(), image->image.end(), m_frameId);
				image->frameId = m_frameId;
				++m_frameId;
				image->timestamp = std::chrono::steady_clock::now();

				Broker::publish<CameraImageTopic>(image);
		}
	}

private:
	std::atomic<bool> m_quit = ATOMIC_VAR_INIT(false);
	std::chrono::milliseconds m_period;
	std::uint64_t m_frameId = 0;
	std::thread m_thread;
};
MESS_SUBSCRIBE(Camera, SystemFSMTopic)