/**
 * @file main.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "broker.h" // the broker for this program is defined in here

#include "printer.h"
#include "stdcin.h"
#include "sensor.h"

#include <thread>

int main(int argc, char **argv)
{
	// Instantiate publishers and subscribers
	Printer printer;
	Sensor sensor;
	Stdcin stdcin;

	// Set the subscribers instances
	Broker::subscriber<0>(printer);
	Broker::subscriber<1>(sensor);

	std::thread stdcinThread([&stdcin](){stdcin.loop();});
	std::thread sensorThread([&sensor](){sensor.loop();});

	stdcinThread.join();
	sensorThread.join();

	return EXIT_SUCCESS;
}