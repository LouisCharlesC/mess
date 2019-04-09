Writing software is a mess, embrace it!
# mess
*mess* is a zero-runtime-cost header-only C++11 library for component based programming (a.k.a. message passing, event based, reactive, publisher-subscriber, signal-slot, dataflow, etc.).

*mess* is designed to be as simple as it can be. It lets you define, at compile-time, a **static** data exchange structure within your program. That is, *mess* lets you tag functions from components (e.i. a classes) within your program so that all other components can call them.

# Briefly
Three calling mechanisms exist in *mess*, allowing push and pull behavior: notifications and publications are pull mechanisms and services are the pull mechanism. When you create a topic in *mess*, you can define it to be a notification, publication or service topic.
* Components can subscribe to notification and publication topics by specifying a callback function. Components can notify() or publish() to the topic to execute the callback function of all subscribing components. Publication callbacks have arguments while notifcation callbacks do not. 
* Service topics have a single associated provider. Components can call() the service to execute the service callback function from the provider. Service callbacks can have arguments and a return value.

# Cost and return
Other messaging frameworks (ROS, Qt, etc.), typically have a runtime cost:
* Memory cost: for the function pointers / std::function objects used to store the callbacks, and the allocation cost for dynamic data structures (std::vector).
* Pointer chasing cost: for calling a function through a function pointer or std::function, for example.
* Virtual function call cost: for calls through std::function for instance.
* Message type conversion cost: for converting from your native processing type to the type your framework forces you to use to exchange data.
* Polymorphic message type resolution cost: for message types that have polymorphic behavior so they can be transmitted through callback functions with a generic signature.
* Thread-safety cost: for the dynamic management of the subscriptions during execution, as callbacks are being called.
* Optimization cost: for any level of indirection or polymorphism that prevent the optimizer from inlining and reasonning about the code.
* Run-time error detection cost: for any level of indirection or polymorphism that prevent type errors to be detected at compile time.

Sometimes, you even pay the compile-time cost of having to use a separate build system (this is true of both ROS and Qt). Admitedly, ROS and Qt offer *much more* functionality than *mess* does. *mess* only deals with moving data and calling functions within your program. *mess* is totally *unthread-safe*, it does not even know what a thread is! Having thread-safety within the framework is overkill, thread-safety is the responsability of each component. Only pay for the thread-safety when you need it!

The goal of *mess* is to provide component-based functionality without compromising **performance**, **readability** and **type-safety**. Of course, you need to pay something to get anything. Here is the cost of *mess*:
* Compilation time cost: there is some amount of meta-programming involved in *mess*, but not that much. Still, this slows down compilation. Also, the framework has to know about every component in the program, and every component has to know about the framework. This induces dependencies between components that would normally not have to be aware of each other. There are ways to limit this dependency to a minimum, and in practice I find it not to be much of a problem.
* Static structure cost: with *mess*, you cannot add or remove subscribers or callbacks on-the-fly. *mess* only lets you define the **static** structure of you program. I find this to be totally acceptable: a program always has a basic static structure. Some programs have a dynamic structure *on top of the static structure*. *mess* takes case of the static part and *let's you build the dynamic part* if you need it. That way, you only pay the cost of a dynamic framework for those parts of your program that benefit from the added flexibiity.

You also pay for a reference to the framework's object that manages the components. Every component must have a reference to this object because function calls are made through this object. This has a runtime cost, of course, and I beg your pardon for stretching the truth when saying *mess* has zero-runtime-cost.

For this measly cost, you get the awesome return of avoiding the cost of the other component based frameworks out there!

# Example
Here is the minimal example usage of *mess* I can come up with. Please check the examples folder to see how to arrange this minimal example into several header and implementation files in order to reduce compilation dependencies.

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
	//                      Topic               Topic again, Provider,  Return, Argument
	template<> struct Topic<Interpret>: Service<Interpret, Interpreter, void, const std::string&> {};
	//                      Topic               Topic again
	template<> struct Topic<Quit>: Notification<Quit> {};
} // mess

// Define components
class Console
{
public:
	Console(Broker& broker):
		m_broker(broker)
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
		if (command == "q")
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

	// Loop until user asks to quit
	console.loop();

	// Rejoice
	return EXIT_SUCCESS;
}
```
