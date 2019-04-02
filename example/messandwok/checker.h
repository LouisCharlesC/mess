/**
 * @file checker.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-25
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "cameratopics.h"
#include "processortopics.h"

#include <algorithm>
#include <deque>
#include <exception>

class Checker
{
public:
	void onPublish(CameraImageTopic, const CameraImageTopic::Message& cameraImage)
	{
		m_cameraImagesQueue.push_back(cameraImage);
	}

	void onPublish(ProcessorImageTopic, const ProcessorImageTopic::Message& processorImage)
	{
		const auto& cameraImage = *std::find_if(m_cameraImagesQueue.cbegin(), m_cameraImagesQueue.cend(), [&processorImage](const CameraImageTopic::Message& cameraImage){return processorImage.frameId == cameraImage->frameId;});
		const bool isOk = std::is_permutation(processorImage.image.cbegin(), processorImage.image.cend(), cameraImage->image.cbegin());
		if (!isOk)
		{
			std::terminate();
		}
	}

private:
	std::deque<CameraImageTopic::Message> m_cameraImagesQueue;
};