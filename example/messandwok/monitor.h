/**
 * @file monitor.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "broker.h"
#include "cameratopics.h"
#include "processortopics.h"
#include "systemtopics.h"

#include <atomic>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <thread>

class Monitor
{
public:
	~Monitor()
	{
		m_thread.join();
	}

	void onPublish(SystemFSMTopic, const SystemFSMTopic::Message& state)
	{
		if (state == SystemFSM::Start)
		{
			m_timeStart = std::chrono::high_resolution_clock::now();
			m_thread = std::thread(&Monitor::loop, this);
		}
		else if (state == SystemFSM::Quit)
		{
			assert(m_thread.joinable());
			m_quit = true;
		}
	}

	void onNotify(CameraImageTopic)
	{
		++m_cameraImageCount;
	}

	void onNotify(ProcessorImageTopic)
	{
		++m_processorImageCount;
	}

	void onNotify(ProcessorImageTimedOutTopic)
	{
		++m_processorImageTimedOutCount;
	}

	void loop()
	{
		auto nextTrigger = std::chrono::high_resolution_clock::now();
		while(!m_quit)
    {
				nextTrigger += m_period;
        std::this_thread::sleep_until(nextTrigger);

				const float elapsed = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-m_timeStart).count());
				std::cout << "Camera and Processor and TimedOut images received at "
				  << static_cast<float>(1000 * m_cameraImageCount) / elapsed
				  << ", "
					<< static_cast<float>(1000 * m_processorImageCount) / elapsed
				  << " and "
					<< static_cast<float>(1000 * m_processorImageTimedOutCount) / elapsed
					<< " fps." << std::endl;
    }
	}

private:
	std::atomic<bool> m_quit = ATOMIC_VAR_INIT(false);
	std::atomic<std::uint64_t> m_cameraImageCount = ATOMIC_VAR_INIT(0);
	std::atomic<std::uint64_t> m_processorImageCount = ATOMIC_VAR_INIT(0);
	std::atomic<std::uint64_t> m_processorImageTimedOutCount = ATOMIC_VAR_INIT(0);
	std::chrono::milliseconds m_period = std::chrono::milliseconds(1000);
	std::chrono::high_resolution_clock::time_point m_timeStart;
	std::thread m_thread;
};