/**
 * @file main.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

// #include "broker.h"
// #include "camera.h"
// #include "cameratopics.h"
// // #include "checker.h"
// #include "messmacros.h"
// #include "monitor.h"
// #include "processor.h"
// #include "processortopics.h"
// #include "systemtopics.h"
// #include "workers.h"

#include <chrono>
#include <cstdlib>
#include <iostream>

// MESS_PUBLICATION_EXISTS(SystemFSMTopic, SystemFSM)
// MESS_SUBSCRIBE_PUBLICATION(SystemFSMTopic, Camera)
// MESS_SUBSCRIBE_PUBLICATION(SystemFSMTopic, Monitor)

// MESS_PUBLICATION_EXISTS(CameraImageTopic, std::shared_ptr<Image>)
// MESS_SUBSCRIBE_PUBLICATION(CameraImageTopic, Processor)
// // MESS_SUBSCRIBE_PUBLICATION(CameraImageTopic, Checker)
// MESS_SUBSCRIBE_NOTIFICATION(CameraImageTopic, Monitor)

// MESS_PUBLICATION_EXISTS(ProcessorImageTopic, Image)
// // MESS_SUBSCRIBE_PUBLICATION(ProcessorImageTopic, Checker)
// MESS_SUBSCRIBE_NOTIFICATION(ProcessorImageTopic, Monitor)

// MESS_NOTIFICATION_EXISTS(ProcessorImageTimedOutTopic)
// MESS_SUBSCRIBE_NOTIFICATION(ProcessorImageTimedOutTopic, Monitor)

int main(int argc, char **argv)
{
	// std::uint8_t ratio = 4;
	// if (argc > 1)
	// {
	// 	ratio = std::atoi(argv[1]);
	// }
	// constexpr auto kCameraPeriod = std::chrono::milliseconds(30);
	// const auto kProcessorDuration = ratio*kCameraPeriod;
	// const std::uint8_t kNbrOfWorkers = ratio;

	// Workers workers(kNbrOfWorkers);
	// auto& submitter = workers.getWorkSubmitter();

	// Camera camera(kCameraPeriod); // 30 images per second
	// Processor processor(submitter, kProcessorDuration);
	// // Checker checker(submitter);
	// Monitor monitor; // print produced, processed and dropped fps

	// Broker::subscriber<0>(camera);
	// Broker::subscriber<1>(processor);
	// // Broker::subscriber<2>(checker);
	// Broker::subscriber<2>(monitor);

	// Broker::publish<SystemFSMTopic>(SystemFSM::Start);

	// char in;
	// std::cin >> in;
	// Broker::publish<SystemFSMTopic>(SystemFSM::Quit);
	// workers.emptyAndStop();

	return EXIT_SUCCESS;
}