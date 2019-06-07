# Readme in progress!

Writing software is a mess, embrace it!
# mess
Just the example code for now, more will come.
## Example
Here is *mess*'s "Hello, world!". You should know that with optimizations enabled, this code compiles to the exact same executable as a plain C++ "Hello, world!" (shown below). This is verified in my tests. Also, have a look at the examples/hellofile.cpp for an example with multiple subscribers.

```c++
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
	// Instantiate the broker
	mess::Broker<LoggerComponent> broker(std::cout);

	// Publish some data to the topic through the broker
	broker.publish<LogTopic>("Hello, world!\n");
}
```
Here is the plain "Hello, world!" the above example compiles equal to:
```c++
#include <iostream>

int main(int argc, char **argv)
{
	std::cout << "Hello, world!\n";
}
```