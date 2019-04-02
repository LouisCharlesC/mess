/**
 * @file sensor.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-04-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "sensor.h"

#include "broker.h"
#include "sensorsubs.h"
#include "stdcintopics.h"

#include <thread>

namespace mess
{
	// Subscriptions
	template<>
	void onNotify<Quit, Sensor>(Sensor& subscriber)
	{
		subscriber.quit();
	}
	// Caches
	const Message<SensorValueCache> TopicTraits<SensorValueCache>::InitialValue;
	// Services
	template<>
	Return<GetPeriod> onCall<GetPeriod>(Provider<GetPeriod>& provider)
	{
		return provider.getPeriod();
	}
	template<>
	Return<SetPeriod> onCall<SetPeriod>(Provider<SetPeriod>& provider, std::chrono::milliseconds period)
	{
		provider.setPeriod(period);
	}
} // mess

Sensor::~Sensor()
{
	m_thread.join();
}

void Sensor::quit()
{
	m_quit = true;
}
std::chrono::milliseconds Sensor::getPeriod()
{
	return m_period.get();
}
void Sensor::setPeriod(std::chrono::milliseconds period)
{
	m_period.set(period);
}
void Sensor::loop()
{
	while(!m_quit)
	{
			std::this_thread::sleep_for(m_period.get());
			++m_data;
			Broker::publish<SensorValueCache>(m_data);
	}
}
