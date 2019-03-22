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
#include "controlmessages.h"
#include "sensormessages.h"
#include "subscribemacros.h"

#include <atomic>
#include <cstdint>
#include <thread>

class Sensor
{
public:
	void onPublish(const QuitTopic::Message& quit, QuitTopic)
	{
		m_quit = quit;
	}

	void loop()
	{
		while(!m_quit)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ++m_data;
				Broker::publish<SensorTopic>(m_data);
    }
	}

private:
	std::atomic<bool> m_quit = ATOMIC_VAR_INIT(false);
	SensorTopic::Message m_data = 0;
};
MESS_SUBSCRIBE_TAG(Sensor, QuitTopic)