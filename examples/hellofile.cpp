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
	std::ofstream ofs("loginfo.txt");
	mess::Broker<ConsoleLogger, FileLogger> broker(std::cout, ofs);
	broker.publish<LogTopic>("Hello, world!\n");
}