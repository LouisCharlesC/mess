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
	void onNotify(QuitTopic)
	{
		m_quit = true;
	}

	GetPeriodService::ReturnType onCall(GetPeriodService)
	{
		return m_period;
	}

	void loop()
	{
		while(!m_quit)
    {
        std::this_thread::sleep_for(m_period);
        ++m_data;
				Broker::publish<SensorTopic>(m_data);
    }
	}

private:
	std::atomic<bool> m_quit = ATOMIC_VAR_INIT(false);
	SensorTopic::Message m_data = 0;
	std::chrono::milliseconds m_period = std::chrono::milliseconds(10);
};
MESS_SUBSCRIBE_NOTIFICATION(Sensor, QuitTopic)
// MESS_ADVERTIZE_TAG(GetPeriodService)