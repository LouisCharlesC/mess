# Readme in progress!

Writing software is a mess, embrace it!
# mess
*mess* is a zero-runtime-cost header-only C++11 library for component based programming (a.k.a. message passing, event driven, reactive, publisher-subscriber, signal-slot, dataflow, observer pattern, etc.).

## Briefly
In *mess*, there exist components, and there exists a Broker. Through the Broker, any component can *notify* a topic, *publish* to a topic and *call* a service to interact with other components. Components need not be aware of each other, they only need to be aware of topics.

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
* Memory cost: for the callable objects (function pointer, std::function, etc.) used to store the subscription callbacks.
* Allocation cost: for the std::vector used to store the variable number of these callbacks.
* Pointer chasing cost: for calling a function through a function pointer or std::function, for example.
* Virtual function call cost: for calls through std::function and subscribe-by-inheritance frameworks.
* Message type conversion cost: to convert your business domain type into the type your framework forces you to use to exchange data.
* Polymorphic message type resolution cost: for message types that have polymorphic behavior so they can be transmitted through callback functions with a generic signature.
* Thread-safety cost: to manage the dynamic messaging structure during execution.
* Optimization cost: for any level of indirection or polymorphism that prevent the optimizer from inlining and reasonning about the code.
* Run-time error detection cost: for any level of indirection or polymorphism that prevent type errors to be detected at compile time.

Sometimes, messaging frameworks even have the compile-time cost of a separate build system (this is true of both ROS and Qt). Admitedly, ROS and Qt offer *much more* functionality than *mess* does. *mess* only deals with moving data and calling functions within your program. *mess* is *non-thread-safe*, it does not even know what a thread is! If you need thread-safety, take care of it within the components that need it. If your code is single-threaded, make it multi-threaded (it will run faster!) and take care of thread-safety within thoses components that need it. *mess* is a good platform for multi-threaded programs.

The goal of *mess* is to provide component-based functionality, and only this, without compromising **performance**, **readability** and **type-safety**. Of course, you need to pay something to get anything. Here is the cost of *mess*:
* Compilation time cost: there is some amount of meta-programming involved in *mess*, but not that much. Still, this slows down compilation. Also, the framework has to know about every component in the program, and every component has to know about the framework. This induces dependencies between components that would normally not have to be aware of each other. There are ways to limit this dependency to a minimum, and in practice I find it not to be much of a problem.
* Static structure cost: with *mess*, you cannot add or remove subscribers or callbacks on-the-fly. *mess* only lets you define the **static** structure of you program. I find this to be totally acceptable: a program always has a basic static structure. Some programs have a dynamic structure *on top of the static structure*. *mess* takes case of the static part and *let's you build the dynamic part* if you need it, anyway you like. That way, you only pay the cost of a dynamic framework for those parts of your program that benefit from the added flexibiity.

For this measly cost, you get the awesome return of avoiding the cost of the other component based frameworks out there!

## Example
Here is *mess*'s "Hello, world!":

```c++
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
```
