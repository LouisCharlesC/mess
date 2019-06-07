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

// Define a component: components receive messages when they are published are pass them to their core
class LoggerComponent
{
public:
	// Define the core: the class that will work on the received messages
	using Core = std::ostream;

	// Define a subscription callback for topic LogTopic
	template<typename Broker>
	static void onPublish(LogTopic, Broker& broker, Core& stream, const char info[])
	{
		stream << info;
	}
};

namespace mess
{
	// Subscribe to topic
	template<> struct Topic<LogTopic>: Subscribers<LoggerComponent> {};
}

int main(int argc, char **argv)
{
	// Using the builder for such a simple example is overkill, I just wanted to show how to use it
	mess::Broker<LoggerComponent>::Builder builder;
	builder.set<LoggerComponent>(std::cout);
	// Instantiate broker: provide core implementations
	const auto broker = builder.build();
	// Publish some data to the topic
	broker.publish<LogTopic>("Hello, world!\n");
}