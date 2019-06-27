# Readme in progress!
Software is a mess, embrace it!

[![Build Status](https://travis-ci.org/LouisCharlesC/mess.svg?branch=master)](https://travis-ci.org/LouisCharlesC/mess)

# mess
*mess* is a zero-runtime-cost header-only C++11 library for publish-subscribe message passing. This is a basic building block for component based programming (a.k.a. message passing, event driven, reactive, publisher-subscriber, signal-slot, dataflow, observer pattern, etc.).

## Briefly
In *mess*, there exist Endpoints, Channels, and a Broker. Endpoints subscribe to Channels and, through the Broker, can publish data. Subscribing means providing a Callback to handle the published data. Subscriptions are managed at compile-time and callbacks are static member functions. Not function pointers, not overriden virtual methods, not std::function objects, just plain, fully-optimizable functions.  
In *mess*, there also are Callbacks and Components. Callbacks are the meeting points of *mess* and your business domain code (the Components). To let one of your Component receive data from *mess*, you have to write an Endpoint class for it.

Using *mess*, you get the full benefit of **structuring** your program into **components** and **loosely coupling** them through message passing, yet, you suffer absolutely **no runtime cost**.

# Example
Here is *mess*'s "Hello, world!". You should know that with optimizations enabled, this code compiles to the exact same executable as a plain C++ "Hello, world!" (shown below). This is verified in my tests. Also, have a look at the other examples in the examples/ folder.

```c++
#include "mess/mess.h"

#include <iostream>

// Create a channel, a channel is just a type
struct Log {};

// This is an Endpoint, a class you have to write to interface mess and a business domain class
class Logger
{
public:
	// The component is your business domain class (for "Hello, world!", an std::ostream suffices)
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
```
Here is the plain "Hello, world!" the above example compiles equal to:
```c++
#include <iostream>

int main(int argc, char **argv)
{
	std::cout << "Hello, world!\n";
}
```

# What is *mess* ?
## What *mess* saves you
All messaging frameworks I know have a runtime cost:
* Memory cost: to store callable objects (function pointers, std::function instances, etc.) used to store the subscription callbacks.
* Processing cost: to iterate through the callbacks.
* Pointer chasing cost: to call the callbacks through function pointers or std::function, for example.
* Virtual function call cost: for calls through std::function and overriden virtual methods.
* Message type conversion cost: to convert your business domain type into the type the framework forces you to use to exchange data.
* Polymorphic message type resolution cost: for message types that have polymorphic behavior so they can be transmitted through callback functions with a generic signature.
* Thread-safety cost: to safely manage the dynamic messaging structure during execution.
* Optimization cost: for any level of indirection or polymorphism that prevent the optimizer from inlining and reasonning about the code.
* Run-time error detection cost: for any level of indirection or polymorphism that prevent type errors to be detected at compile time.

Sometimes, messaging frameworks even have the compile-time cost of a separate build system.

## What *mess* does not offer
Admitedly, message-passing frameworks typically offer *much more* functionality than *mess* does. *mess* only deals with moving data and calling functions within your program. *mess* is *non-thread-safe*, it does not even know what a thread is! If you need thread-safety, take care of it within the components that need it. If your code is single-threaded, make it multi-threaded (it will run faster!) and take care of thread-safety within thoses components that need it. *mess* is a good platform for multi-threaded programs.

The goal of *mess* is to provide component-based functionality, and only this, without compromising **performance**, **readability** and **type-safety**. Of course, you need to pay something to get anything. Here is the cost of *mess*:
* Compilation time and dependency cost: there is some amount of meta-programming involved in *mess*, but not that much. Still, this slows down compilation. Also, the framework has to know about every component in your program, and every component has to know about the framework. This induces dependencies between components that would normally not have to be aware of each other. There are ways to limit this dependency to a minimum, and in practice I find it not to be much of a problem. If this is a concern, please see the examples/split example.
* Static structure cost: with *mess*, you cannot add or remove subscribers or callbacks on-the-fly. *mess* only lets you define the **static** structure of you program. I find this to be totally acceptable: a program always has a basic static structure. Some programs have a dynamic structure *on top of the static structure*. *mess* takes case of the static part and *let's you build the dynamic part* if you need it, anyway you like. That way, you only pay the cost of a dynamic framework for those parts of your program that benefit from the added flexibiity.

# How to use *mess*
0. Decompose your program, identify natural boundaries
1. Define Channels
2. Define Endpoints
	2.1. Define Callbacks
3. Subscribe Endpoints to Channels
4. Instantiate the Broker
5. Publish data

# Anatomy of a Callback

# Splitting

# 