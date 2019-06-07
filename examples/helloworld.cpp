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

// Define a topic
struct LogTopic {};

// Define a component: the role of a component is to bridge between topics and cores.
class LoggerComponent
{
public:
	// Declare the core: the class that will work on the received messages
	using Core = std::ostream;

	// Define a subscription callback for topic LogTopic
	template<typename Broker>
	static void onPublish(LogTopic, Broker& broker, Core& stream, const char info[])
	{
		stream << info; // pass the received data to the core
		// you can use the broker to publish the result of your computations if needed
	}
};

namespace mess
{
	// Declare subscribers for the topic
	template<> struct Topic<LogTopic>: Subscribers<LoggerComponent> {};
}

int main(int argc, char **argv)
{
	// Using the builder for such a simple example is overkill, I just wanted to show how to use it
	mess::Broker<LoggerComponent>::Builder builder;
	builder.set<LoggerComponent>(std::cout); // set a reference to the component's core
	
	const auto broker = builder.build(); // get the broker from the builder

	// Publish some data to the topic through the broker
	broker.publish<LogTopic>("Hello, world!\n");
}