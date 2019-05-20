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

struct LogInfo {};

class Logger
{
public:
	using Core = std::ostream;

	template<typename Broker>
	static void onPublish(LogInfo, Broker& broker, std::ostream& stream, const char info[])
	{
		stream << info;
	}
};

namespace mess
{
	template<> struct Topic<LogInfo>: Subscribe<Logger> {};
}

int main(int argc, char **argv)
{
	mess::Broker<Logger> broker(std::cout);
	broker.publish<LogInfo>("Hello, world!\n");
}