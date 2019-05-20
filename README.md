Writing software is a mess, embrace it!
# mess
*mess* is a zero-runtime-cost header-only C++11 library for component based programming (a.k.a. message passing, event driven, reactive, publisher-subscriber, signal-slot, dataflow, observer pattern, etc.).

## Briefly
Two calling mechanisms exist in *mess*: publications and services. When you create a topic in *mess*, you can define it to be a publication or a service topic. Publications can have multiple subscribers, whereas services have a single provider. Publications do not have a return value, services can have one.

## Cost and return
Other messaging frameworks (ROS, Qt, etc.), typically have a runtime cost:
* Memory cost: for the callable objects (function pointer, std::function, etc.) used to store the subscription callbacks.
* Allocation cost: for the std::vector used to store the variable number of these callbacks.
* Pointer chasing cost: for calling a function through a function pointer or std::function, for example.
* Virtual function call cost: for calls through std::function for instance.
* Message type conversion cost: for converting from your native processing type to the type your framework forces you to use to exchange data.
* Polymorphic message type resolution cost: for message types that have polymorphic behavior so they can be transmitted through callback functions with a generic signature.
* Thread-safety cost: for the dynamic management of the subscriptions during execution, as callbacks are being called.
* Optimization cost: for any level of indirection or polymorphism that prevent the optimizer from inlining and reasonning about the code.
* Run-time error detection cost: for any level of indirection or polymorphism that prevent type errors to be detected at compile time.

Sometimes, you even pay the compile-time cost of having to use a separate build system (e.g. ROS, Qt). Admitedly, ROS and Qt offer *much more* functionality than *mess* does. *mess* only deals with moving data and calling functions within your program. *mess* is totally *unthread-safe*, it does not even know what a thread is! Having thread-safety within the framework is overkill, thread-safety is the responsability of each component. Only pay for the thread-safety when you need it!

The goal of *mess* is to provide component-based functionality without compromising **performance**, **readability** and **type-safety**. Of course, you need to pay something to get anything. Here is the cost of *mess*:
* Compilation time cost: there is some amount of meta-programming involved in *mess*, but not that much. Still, this slows down compilation. Also, the framework has to know about every component in the program, and every component has to know about the framework. This induces dependencies between components that would normally not have to be aware of each other. There are ways to limit this dependency to a minimum, and in practice I find it not to be much of a problem.
* Static structure cost: with *mess*, you cannot add or remove subscribers or callbacks on-the-fly. *mess* only lets you define the **static** structure of you program. I find this to be totally acceptable: a program always has a basic static structure. Some programs have a dynamic structure *on top of the static structure*. *mess* takes case of the static part and *let's you build the dynamic part* if you need it, anyway you like. That way, you only pay the cost of a dynamic framework for those parts of your program that benefit from the added flexibiity.

For this measly cost, you get the awesome return of avoiding the cost of the other component based frameworks out there!

## Example
Here is *mess*'s "Hello, world!":

```c++#include "mess/mess.h"

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
