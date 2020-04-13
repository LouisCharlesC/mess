# Readme in progress!
# *Software is a mess, embrace it!*

[![Build Status](https://travis-ci.org/LouisCharlesC/mess.svg?branch=master)](https://travis-ci.org/LouisCharlesC/mess)
[![Build status](https://ci.appveyor.com/api/projects/status/3550cw0y96igwlye/branch/master?svg=true)](https://ci.appveyor.com/project/LouisCharlesC/mess/branch/master)

# mess
*mess* is a compile-time header-only C++17 library for dataflow programing (a.k.a. message passing, event driven, actors, reactive, publisher-subscriber, signal-slot, observer pattern, etc.). Execution of the dataflow is runtime, but its setup is compile-time. This gives you the flexibility of a message passing framework and the performance of plain C++ function calls!

# WIP
*mess* is currently under development. This version is out there for me to gather feedback about the terminology, usage and useful features. I am working on version 1.0, which as a bare minimum will:
1. Allow the use of member functions.
1. Resolve dependencies between values: produce intermediate values needed to produce the pulled value.
1. Order the calls between the dependencies to avoid use-after-move when multple values have the same dependency and some callback would benefit from moving the value in.

It is foreseen that future versions might include:
1. Concurrency (through coroutines?).
1. Multi-theading.
1. Dynamic subscriptions.

# Example
Here is *mess*'s "Hello, world!". You should know that with optimizations enabled, this code compiles to the exact same executable as a plain C++ "Hello, world!" (shown below). This is verified in the tests. Also, have a look at the other examples in the examples/ folder.

```c++
#include <mess/mess.h>

#include <iostream>

namespace {
std::ostream& coutLogger()
{
    return std::cout;
}
std::ostream& cerrLogger()
{
    return std::cerr;
}
}

struct Logger;

namespace mess
{
    template<> struct
    ToProduce<Logger>:
        Call<coutLogger>, // replace with: "Call<cerrLogger>," to change the logger!
        WithNoArgs
    {};
}

int main(int argc, char **argv)
{
     mess::Frame<>::pull<Logger>() << "Hello, world!\n";
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
* Run-time error detection cost: when polymorphism prevents type errors to be detected at compile time.
* etc.

Sometimes, messaging frameworks even have the compile-time cost of a separate build system (ROS, Qt).

## What *mess* does not offer
Admitedly, message-passing frameworks typically offer *much more* functionality than *mess* does. *mess* only deals with moving data and calling functions within your program. *mess* is **not thread-safe**, it does not even know what a thread is! If you need thread-safety, take care of it within the components that need it.

The goal of *mess* is to provide component-based functionality, and only this, without compromising **performance**, **readability** and **type-safety**. Of course, you need to pay something to get anything. Here is the cost of *mess*:
* Compilation time: there is some amount of meta-programming involved in *mess*, but not that much. Still, this slows down compilation.
* Static structure: with *mess*, you cannot add or remove subscribers or callbacks on-the-fly. *mess* only lets you define the **static** structure of you program. I find this to be totally acceptable: a program always has a basic static structure. Some programs have a dynamic structure *on top of the static structure*. *mess* takes case of the static part and *let's you build the dynamic part* if you need it, anyway you like. That way, you only pay the cost of a dynamic framework for those parts of your program that benefit from the added flexibiity.
