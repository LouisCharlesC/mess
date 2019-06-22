/**
 * @file helloworld.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-04-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "mess/mess.h"

#include <iostream>

// Define a channe;
struct Log {};

// Define an endpoint: an endpoint is where mess and your domain-specific code meet
class Logger
{
public:
	// Declare the component for this endpoint, your domain-specific class that actually does the work
	using Component = std::ostream;

	// Define a subscription callback for the channel.
	// The 3 first arguments are imposed by mess, you can add anything you like after that
	template<typename Broker>
	static void onPublish(Log, Broker& broker, Component& logger, const char msg[])
	{
		logger << msg; // pass the message to your code
	}
};

namespace mess
{
	// Define subscribers for the channel (there could be more than one subscriber)
	template<> struct Channel<Log>: Subscribers<Logger> {};
}

int main(int argc, char **argv)
{
	// Instantiate the broker, pass std::cout as the Logger's component
	mess::Broker<Logger> broker(std::cout);

	// Publish to the Log topic through the broker
	broker.publish<Log>("Hello, world!\n");
}