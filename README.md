Writing software is a mess, embrace it!
# mess
*mess* is a zero-runtime-cost header-only C++11 library for component based programming (a.k.a. message passing, event driven, reactive, publisher-subscriber, signal-slot, dataflow, observer pattern, etc.).

*mess* is designed to be as simple as it can be. It lets you define, at **compile-time**, a **static** data exchange structre within your program. That is, *mess* lets you tag functions from components (e.i. a classes) within your program so that all other components can call them. This is done in *pure C++11, without any use of macros*.

## Briefly
In *mess*, there exist components, and there exists a Broker. Through the Broker, any component can *notify* a topic, *publish* to a topic and *call* a service to interact with other components. Components need not be aware of each other, they only need to be aware of topics.

Notifications and publications execute as many callback functions as there are components that subscribed to the topic. Services have a single provider and its callback function is executed when the service is called. Publications have arguments while notifications do not. Services can have arguments and can have a return value. Publications and notifications do not have a return value.

Creating notification, publication and service topics and subscribing to them *is done at compile-time*. There is absolutely no run-time operation involved. Zero, runtime, cost. To put it differently, imagine the topic "PizzaReady" and the components "Leonardo", "Michelangelo", "Raphael", and "Donatello" exist. After the components have subscribed to the topic and registered their ```eat()``` member function callback, writing:
```c++
broker.notify<PizzaReady>();
```
is **exactly** the same as writing:
```c++
leonardo.eatPizza();
michelangelo.eatPizza();
raphael.eatPizza();
donatello.eatPizza();
```
except that the former is concise and expressive, and if there are several places in your code where eating pizza is appropriate and at some point in the future Splinter becomes interested in pizza (you thought it was 100% impossible, but it happenned!), you only need to add a few lines of code to subscribe Splinter to the topic rather than track down every place you should add splinter.eatPizza(); (and pass a reference to splinter all the way down to where these calls happen).

## Cost and return
Messaging frameworks (ROS, Qt, and all such projects you find on github), have a runtime cost:
* Memory cost: for the function pointers / std::function objects used to store the callbacks, and the allocation cost for dynamic data structures (std::vector).
* Pointer chasing cost: for calling a function through a function pointer or std::function stored in a vector.
* Virtual function call cost: for calls through std::function and subscribe-by-override frameworks.
* Message type conversion cost: for converting from your business domain type to the type your framework forces you to use to exchange data.
* Polymorphic message type resolution cost: for message types that have polymorphic behavior so they can be transmitted through generic callback functions.
* Thread-safety cost: for managing the dynamic structure during execution.
* Optimization cost: for any level of indirection or polymorphism that prevent the optimizer from inlining code or anyhow reason about it.
* Run-time error detection cost: for any level of indirection or polymorphism that prevent type errors to be detected at compile time.

Sometimes, messaging frameworks even have the compile-time cost of a separate build system (this is true of both ROS and Qt). Admitedly, ROS and Qt offer *much more* functionality than *mess* does. *mess* only deals with moving data and calling functions within your program. *mess* is *non-thread-safe*, it does not even know what a thread is! If you need thread-safety, take care of it within the components that need it. If your code is single-threaded, make it multi-threaded (it will run faster, you know) and take care of thread-safety within thoses components that need it. *mess* is a good platform for multi-threaded programs.

The goal of *mess* is to provide component-based functionality, and only this, without compromising **performance**, **readability** and **type-safety**. Of course, you need to pay something to get anything. Here is the cost of *mess*:
* Compilation time cost: there is some amount of template meta-programming involved in *mess*, that might slow down compilation. Basically, the compiler's job becomes more complicated as the number of components and topics increase. Also, the framework has to know about every component in the program, and every component has to know about the framework. This induces dependencies between components that would normally not have to be aware of each other. There are ways to limit this dependency to a minimum, and in practice I find it not to be much of a problem.
* Static structure cost: with *mess*, you cannot add or remove subscribers or callbacks on-the-fly. *mess* only lets you define the **static structure** of you program. I find this to be totally acceptable: a program always has a basic static structure. Some programs have a dynamic structure *on top of the static structure*. *mess* takes case of the static part and *let's you build the dynamic part* if you need it. That way, you only pay the cost of a dynamic framework for those parts of your program that benefit from the added flexibiity.

You also pay for a reference to the broker object that manages the components. Every component must have a reference to this broker because function calls are made through it. This has a runtime cost, of course, and I beg your pardon for stretching the truth when saying *mess* has zero-runtime-cost.

For this measly cost, you get the awesome return of avoiding the cost of the other component based frameworks out there!

## Example
Here is the minimal example of *mess* I can come up with. Please check the examples folder to see how to arrange this minimal example into several header and implementation files in order to reduce compilation dependencies.

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
