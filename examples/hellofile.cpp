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

struct LogTopic {};

struct LoggerComponent
{
	using Core = std::ostream;

	template<typename Broker>
	static void onPublish(LogTopic, Broker& broker, Core& stream, const char* info)
	{
		stream << info;
	}
};
struct ConsoleLogger: LoggerComponent {};
struct FileLogger: LoggerComponent {};

namespace mess
{
	template<> struct Topic<LogTopic>: Subscribers<ConsoleLogger, FileLogger> {};
}

int main(int argc, char **argv)
{
	// Instantiate FileLogger's core
	std::ofstream ofs("log.txt");

	// Using the builder for such a simple example is overkill, I just wanted to show how to use it
	mess::Broker<ConsoleLogger, FileLogger>::Builder builder;
	builder.set<ConsoleLogger>(std::cout); // set the reference to ConsoleLogger's core
	builder.set<FileLogger>(ofs); // set a reference to FileLogger's core
	
	const auto broker = builder.build(); // get the broker from the builder

	broker.publish<LogTopic>("Hello, world!\n");
}