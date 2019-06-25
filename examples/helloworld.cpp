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

// Create a channel, a channel is just a type
struct Log {};

// This is an Endpoint, a class you have to write to interface mess and a business domain class
class Logger
{
public:
	// The component is your business domain class (for "Hello, wordl!", an std::ostream suffices)
	using Component = std::ostream;

	// This is a callback for the Log channel, the signature is partly imposed by mess
	template<typename Broker>
	static void onPublish(Log, Broker& broker, Component& logger, const char msg[])
	{
		// Here, mess and your code meet!
		// Typically you 1) pass the data to the component
		logger << msg;
		// and 2) use the broker to publish any result you want to propagate.
		// broker.publish<AChannel>(someData);
	}
};

namespace mess
{
	// This is how you tell mess which endpoints subscribe to a channel.
	template<> struct Channel<Log>: Subscribers<Logger> {};
}

int main(int argc, char **argv)
{
	// Instantiate the broker. Remember the component ? When you instantiate the broker, you have to provide it with an instance of each component in your program (std::cout is an instance of std::ostream).
	mess::Broker<Logger> broker(std::cout);

	// Publish to the Log channel through the broker
	broker.publish<Log>("Hello, world!\n");
}