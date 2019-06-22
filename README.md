# Readme in progress!

Writing software is a mess, embrace it!
# mess
Just the example code for now, more will come.
## Example
Here is *mess*'s "Hello, world!". You should know that with optimizations enabled, this code compiles to the exact same executable as a plain C++ "Hello, world!" (shown below). This is verified in my tests. Also, have a look at the other examples in the examples/ folder.

```c++
#include "mess/mess.h"

#include <iostream>

// Define a channe;
struct Log {};

// Define an endpoint: an endpoint is where mess and your domain-specific code meet
class Logger
{
public:
	// Declare the component for this endpoint, your domain-specific class that actually does the work
	using Component = std::ostream;

	// Define a subscription callback for the channel.
	// The 3 first arguments are imposed by mess, you can add anything you like after that
	template<typename Broker>
	static void onPublish(Log, Broker& broker, Component& stream, const char msg[])
	{
		stream << msg; // pass the message to your code
	}
};

namespace mess
{
	// Define subscribers for the channel (there could be more than one subscriber)
	template<> struct Channel<Log>: Subscribers<Logger> {};
}

int main(int argc, char **argv)
{
	// Instantiate the broker, pass std::cout as the Logger's component
	mess::Broker<Logger> broker(std::cout);

	// Publish to the Log topic through the broker
	broker.publish<Log>("Hello, world!\n");
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