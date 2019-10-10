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

// Create a channel, a channel is just a type, put it in a special namespace if you please
struct Log {};

// This is an Endpoint, a class you have to write to interface mess and a business domain class
class Logger
{
public:
	// The component is your business domain class (for "Hello, wordl!", an std::ostream suffices)
	using Core = std::ostream;

	// This is a callback for the Log channel, the first three arguments are imposed by mess, the rest is up to you but must match whatever is sent into the channel by the publishers!
	template<typename Broker>
	static void receive(Log, const Broker& broker, Core& stream, const char msg[])
	{
		// Here, mess and your code meet!
		// Typically you 1) pass the data to the component
		stream << msg;
		// and 2) use the broker to publish any result you want to propagate.
		// broker.publish<AChannel>(someData);
	}
};

int main(int argc, char **argv)
{
	// Instantiate the broker. Remember the component ? When you instantiate the broker, you have to provide it with an instance of each component in your program (std::cout is an instance of std::ostream).
	mess::Hub<Logger> hub(std::cout);

	// Publish to the Log channel through the broker
	hub.send<Log>("Hello, world!\n");
}