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

#include "camera.h"
#include "filter.h"
#include "monitor.h"
#include "systemmessages.h"

#include "workers.h"

#include <chrono>
#include <cstdlib>
#include <iostream>

int main(int argc, char **argv)
{
	std::uint8_t nbrOfWorkers = 4;
	if (argc > 1)
	{
		nbrOfWorkers = std::atoi(argv[1]);
	}
	Workers workers(nbrOfWorkers);
	auto& submitter = workers.getWorkSubmitter();

	Camera camera(std::chrono::milliseconds(30)); // 30 images per second
	Filter filter(submitter); // 50 ms to process
	Monitor monitor; // print produced, processed and dropped fps

	Broker::subscriber<0>(camera);
	Broker::subscriber<1>(filter);
	Broker::subscriber<2>(monitor);

	Broker::publish<SystemFSMTopic>(SystemFSM::Start);

	char in;
	std::cin >> in;
	Broker::publish<SystemFSMTopic>(SystemFSM::Quit);
}