/**
 * @file helloworld.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-06-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "mess/mess.h"

#include <iostream>

struct Log {};

struct Logger
{
	using Component = std::ostream;

	template<typename Broker>
	static void onPublish(Log, Broker& broker, Component& logger, const char msg[])
	{
		logger << msg;
	}
};
struct InfoLogger: Logger {};
struct ErrorLogger: Logger {};

namespace mess
{
	template<> struct Channel<Log>: Subscribers<InfoLogger, ErrorLogger> {};
}

int main(int argc, char **argv)
{
	// Build the Broker
	mess::Broker<InfoLogger>::Builder builder;
	builder.set<InfoLogger>(std::cout); // set Logger's component
	builder.set<ErrorLogger>(std::cerr); // set ErrorLogger's component, even if it is unused
	auto broker = builder.build(); // get the broker from the builder
	// Publish to the Log topic through the broker
	broker.publish<Log>("Hello, world!\n");
}