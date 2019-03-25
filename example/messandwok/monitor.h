

#include "broker.h"
#include "cameramessages.h"
#include "systemmessages.h"

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
		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}

	void onPublish(const SystemFSMTopic::Message& state, SystemFSMTopic)
	{
		if (state == SystemFSM::Start)
		{
			m_thread = std::thread(&Monitor::loop, this);
		}
		else if (state == SystemFSM::Quit)
		{
			assert(m_thread.joinable());
			m_quit = true;
			m_thread.join();
		}
	}

	void onPublish(const CameraImageTopic::Message&, CameraImageTopic)
	{
		++m_imageCount;
	}

	void loop()
	{
		auto nextTrigger = std::chrono::steady_clock::now();
		while(!m_quit)
    {
        std::this_thread::sleep_until(nextTrigger);
				nextTrigger += m_period;

				std::cout << "Images received at " << static_cast<float>(1000 * m_imageCount) / static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-m_timeStart).count()) << " fps." << std::endl;
    }
	}

private:
	std::atomic<bool> m_quit = ATOMIC_VAR_INIT(false);
	std::uint64_t m_imageCount = 0;
	std::chrono::milliseconds m_period = std::chrono::milliseconds(1000);
	std::chrono::high_resolution_clock::time_point m_timeStart = std::chrono::high_resolution_clock::now();
	std::thread m_thread;
};
MESS_SUBSCRIBE(Monitor, SystemFSMTopic)
MESS_SUBSCRIBE(Monitor, CameraImageTopic)