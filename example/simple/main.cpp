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

#include "broker.h"
#include "printer.h"
#include "stdcin.h"
#include "sensor.h"

#include <cstdlib>
#include <thread>

int main(int argc, char **argv)
{
	// Instantiate subscribers, sinks and cache and service providers
	Printer printer;
	Sensor sensor;
	Stdcin stdcin;

	// Set the subscribers instances
	Broker::subscriber<0>(printer);
	Broker::subscriber<1>(sensor);
	Broker::subscriber<2>(stdcin);

	return EXIT_SUCCESS;
}