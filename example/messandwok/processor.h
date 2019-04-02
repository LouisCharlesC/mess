/**
 * @file processor.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "cameratopics.h"
#include "processortopics.h"
#include "resource.h"
#include "workers.h"

#include <chrono>

class Processor
{
public:
	Processor(Workers::WorkSubmitter& submitter, std::chrono::milliseconds duration):
		m_submitter(submitter),
		m_duration(duration)
	{}

	void onPublish(CameraImageTopic, const CameraImageTopic::Message& message)
	{
		m_submitter([this, message](){this->shuffle(message);});
	}

	void shuffle(CameraImageTopic::Message message)
	{
		if (isTimedOut(message))
		{
			Broker::publish<ProcessorImageTimedOutTopic>(message->frameId);
			return;
		}

		const auto timeIn = std::chrono::high_resolution_clock::now();

		auto shuffledImage = m_images.get();
		shuffledImage->frameId = message->frameId;

		std::copy(message->image.cbegin(), message->image.cend(), shuffledImage->image.begin());
		std::random_shuffle(shuffledImage->image.begin(), shuffledImage->image.end());
		while (std::chrono::high_resolution_clock::now()<(timeIn+m_duration))
		{
			++shuffledImage->image.front();
		}
		// std::random_shuffle(shuffledImage->image.begin(), shuffledImage->image.end());
		shuffledImage->timestamp = std::chrono::steady_clock::now();
		Broker::publish<ProcessorImageTopic>(*shuffledImage);
	}

private:
	static bool isTimedOut(const CameraImageTopic::Message& message)
	{
		return std::chrono::steady_clock::now()>(message->timestamp+std::chrono::milliseconds(300));
	}

	Workers::WorkSubmitter& m_submitter;
	std::chrono::milliseconds m_duration;
	mess::Resource<Image> m_images;
};