
#include "workers.h"

#include <chrono>
#include <thread>

class Filter
{
public:
	Filter(Workers::WorkSubmitter& submitter, std::chrono::milliseconds duration):
		m_submitter(submitter),
		m_duration(duration)
	{}

	void onPublish(const CameraImageTopic::Message& message, CameraImageTopic)
	{
		m_submitter([this, &message](){this->filter(message);});
	}

	void filter(const CameraImageTopic::Message& message)
	{
		const auto timeIn = std::chrono::high_resolution_clock::now();

		// filter

		std::this_thread::sleep_until(timeIn+m_duration);
	}

private:
	Workers::WorkSubmitter& m_submitter;
	std::chrono::milliseconds m_duration;
};
MESS_SUBSCRIBE(Filter, CameraImageTopic)