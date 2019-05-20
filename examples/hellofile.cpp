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

struct LogInfo {};

struct Logger
{
	using Core = std::ostream;

	template<typename Broker>
	static void onPublish(LogInfo, Broker& broker, std::ostream& os, const char* info)
	{
		os << info;
	}
};
struct ConsoleLogger: Logger {};
struct FileLogger: Logger {};

namespace mess
{
	template<> struct Topic<LogInfo>: Subscribe<ConsoleLogger, FileLogger> {};
}

int main(int argc, char **argv)
{
	std::ofstream ofs("loginfo.txt");
	mess::Broker<ConsoleLogger, FileLogger> broker(std::cout, ofs);
	broker.publish<LogInfo>("Hello, world!\n");
}