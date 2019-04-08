Writing software is a mess, embrace it!
# mess
*mess* is a zero-runtime-cost header-only C++11 library for component based programming (use whatever name you prefer: message passing, event based, reactive, publisher-subscriber, signal-slot, etc.).

*mess* is designed to be as simple as it can be. It lets you define, at compile-time, a **static** data exchange structure within your program. That is, *mess* lets you tag functions from components (e.i. a classes) within your program so that all other components can call them.

## Cost and return
Other messaging frameworks (ROS, Qt, etc.), typically have a runtime cost:
  * Memory cost: for the function pointers / std::function objects. Add the allocation cost for dynamic data structures (std::vector).
  * Pointer chasing cost: for calling a function through a function pointer or std::function, for example.
  * Virtual function call cost: for calls through std::function for instance.
  * Polymorphic message type resolution cost: for passing data through functions with a generic signature.
  * Optimization cost: for any level of indirection or polymorphism that prevent the optimizer from inlining and reasonning about the code.
  * Run-time error detection cost: for any level of indirection or polymorphism that prevent type errors to be detected at compile time.

The goal of *mess* is to provide component-based functionality without compromising **performance**, **readability** and **type-safety**. Of course, you need to pay something to get anything. Here is the cost of *mess*:
* Compilation time cost: there is some amount of meta-programming involved in *mess*, but not that much. Still, this slows down compilation. Also, the framework has to know about every component in the program, and every component has to know about the framework. This induces dependencies between components that would normally not have to be aware of each other. There are ways to limit this dependency to a minimum, and in practice I find it not to be much of a problem.
* Static structure cost: with *mess*, you cannot add or remove subscribers or callbacks on-the-fly. *mess* only lets you define the **static** structure of you program. I dinf this to be totally acceptable: a program always has a basic static structure. Some programs have a dynamic structure *on top of the static structure*. *mess* takes case of the static part and *let's you build the dynamic part* if you need it. That way, you only pay the cost of a dynamic framework for those parts of your program that benefit from the added flexibiity.

You also pay for a reference to the framework's object that manages the components. Every component must have a reference to this object because function calls are made through this object. This has a runtime cost, of course, and I beg your pardon for stretching the truth when saying *mess* has zero-runtime-cost.

#Example
Here is the minimal example usage of *mess* I can come up with:

```c++
#include "mess/mess.h"

#include <cstddef>
#include <iostream>
#include <string>

// Declare components
class Console;
class Interpreter;

// Alias for the broker used in this program
using Broker = mess::BrokerTemplate<Console, Interpreter>;

// Declare topics
struct Interpret;
struct Quit;
// Define topic traits
namespace mess
{
	template<> struct Topic<Interpret>: Service<Interpret, Interpreter, void, const std::string&> {};
	template<> struct Topic<Quit>: Notification<Quit> {};
} // mess

// Define components
class Console: mess::Component<Console>
{
public:
	Console(Broker& broker):
		mess::Component(broker)
	{}

	void quit()
	{
		m_quit = true;
	}

	void loop()
	{
		std::string input;
		while (!m_quit)
		{
			std::cin >> input;
			m_broker.call<Interpret>(input);
		}
	}

private:
	Broker& m_broker;
	bool m_quit = false;
};

class Interpreter
{
public:
	Interpreter(Broker& broker):
		m_broker(broker)
	{}

	void interpret(const std::string& command)
	{
		if (command == "q" || command == "quit")
		{
			m_broker.notify<Quit>();
			std::cout << "Quit." << std::endl;
		}
		else
		{
			std::cout << "Echo: " << command << std::endl;
		}
	}

private:
	Broker& m_broker;
};

// Define callbacks
namespace mess
{
	template<> template<>
	void Topic<Quit>::Notification::onNotify(Console& subscriber)
	{
		subscriber.quit();
	}

	template<>
	void Topic<Interpret>::Service::onCall(Interpreter& provider, const std::string& command)
	{
		provider.interpret(command);
	}
} // mess

int main(int argc, char **argv)
{
	// Instantiate broker
	Broker broker;

	// Instantiate components
	mess::Component<Console> console(broker);
	mess::Component<Interpreter> interpreter(broker);

	// Enter loop
	console.loop();

	// Rejoice
	return EXIT_SUCCESS;
}
```
