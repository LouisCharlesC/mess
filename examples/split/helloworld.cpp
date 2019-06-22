/**
 * @file main.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-06-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "broker.h"
#include "logger.h"
#include "logsub.h"

#include <iostream>

int main(int argc, char **argv)
{
	// Instantiate the broker
	Broker broker(std::cout);

	// Publish to the channel through the broker
	broker.publish<Log>("Hello, world!\n");
}