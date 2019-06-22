/**
 * @file hellofile.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-04-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "mess/mess.h"

#include <fstream>
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
struct ConsoleLogger: Logger {};
struct FileLogger: Logger {};

namespace mess
{
	template<> struct Channel<Log>: Subscribers<ConsoleLogger, FileLogger> {};
}

int main(int argc, char **argv)
{
	// Instantiate FileLogger's core
	std::ofstream ofs("log.txt");

	// Using the builder for such a simple example is overkill, I just wanted to show how to use it
	mess::Broker<ConsoleLogger, FileLogger>::Builder builder;
	builder.set<ConsoleLogger>(std::cout); // set ConsoleLogger's component
	builder.set<FileLogger>(ofs); // set FileLogger's component
	
	auto broker = builder.build(); // get the broker from the builder
	// Publish to the Log topic through the broker
	broker.publish<Log>("Hello, world!\n");
}