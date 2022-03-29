# *Software is a mess, embrace it !*

[![Build Status](https://travis-ci.org/LouisCharlesC/mess.svg?branch=master)](https://travis-ci.org/LouisCharlesC/mess)
[![Build status](https://ci.appveyor.com/api/projects/status/3550cw0y96igwlye/branch/master?svg=true)](https://ci.appveyor.com/project/LouisCharlesC/mess/branch/master)

*mess* is a compile-time, header-only C++17 library for dataflow programing (a.k.a. message passing, event-driven, actors, reactors, publisher-subscriber, signal-slot, observer pattern, etc.).  
Tons of such frameworks exist, but *mess* is 100% non-intrusive and optimized away by the compiler (see section [Hello world](#Hello-world) for a demo).

## Briefly

*mess* lets you name the values your program can compute so you can express the dependencies between them. This forms your program's dataflow. Once this is setup, you can ask *mess* to produce any named value. The execution of the dataflow is runtime, but its setup is compile-time. This gives you the flexibility of a message passing framework and the performance of plain C++ function calls !

As an example, here is how to tell *mess* that a value called `FilteredValue` exists, and that it can be computed by calling the member function `filter` from a value called `LowPassFilter` with as its sole argument the value called `GoodLowPassParameter` (something like `FilteredValue = LowPassFilter.filter(GoodLowPassParameter);`):

```c++
struct FilteredValue:
	mess::IsPulledFrom<&IFilter::filter>,
	mess::OnInstance<LowPassFilter>,
	mess::WithArgument<GoodLowPassParameter>
{};
```

You can get `FilteredValue` by calling `mess::pull<FilteredValue>()`. The function is called `pull` because you explicitly ask for the value to be produced and *mess* will compute any other value it needs to do so. If the dependencies cannot be resolved or the types don't fit, your program won't compile. *mess* does not allow `push`ing values (i.e. producing every value that depends on the `push`ed one). I'm not sure if it's possible, or desirable.

## WIP

*mess* is currently under development. This version is out there for me to gather feedback about the terminology, usage and useful features. I am working on version 1.0, which as a bare minimum will:

1. Compute dependencies only once, even if they are needed by more than one of the pulled values.
1. Order the calls so that computed dependencies can be moved if possible, without risking use-after-move or any other bad surprises.
1. Split the calls into independent stages that you can choose to parallelize using your favorite library.

It is foreseen that future versions might:

1. Facilitate concurrency (coroutines?).
1. Transparently save intermediate values inside *mess* to share the computations between several calls.
1. Allow pushing values, if possible.
1. Allow calls to overload sets (non-resolved overload) and function templates.
1. Provide nice compilation errors rather than the typical template instantiation error messages.

## Hello world

Here is *mess*'s “Hello, world!”. You should know that with optimizations enabled, this code compiles to the same executable as a plain C++ “Hello, world!” (shown below). This is verified in the tests.  
I do apologize function pointer casting, but I think that using an operator from the `std` namespace showcases the non-intrusiveness of *mess*! And it also demonstrates a limitation: you must manually resolve overloads and provide template arguments.  
So, there it is: the overload-resolved function pointer to the template-arguments-provided std::operator<<().

```c++
#include <mess/mess.h>

#include <iostream>

static const char* kHelloWorld = "Hello, world!\n";

using PrintFnPtr = std::basic_ostream<char, std::char_traits<char>>&(*)(std::basic_ostream<char, std::char_traits<char>>&, const char*);
static constexpr PrintFnPtr print = std::operator<< <std::char_traits<char> >;

struct PrintHelloWorld:
	mess::IsPulledFrom<print>,
	mess::WithArguments<
		mess::IsPulledFrom<&std::cout>,
		mess::IsPulledFrom<&kHelloWorld>>
{};

int main()
{
	 mess::pull<PrintHelloWorld>();
}
```

Here is the plain “Hello, world!” the above example compiles equal to:

```c++
#include <iostream>

int main(int argc, char **argv)
{
	std::cout << "Hello, world!\n";
}
```

## Why use *mess*

### What *mess* saves you

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

Sometimes, dataflow frameworks even have the compile-time cost of a separate build system (ROS, Qt).

### What *mess* does not offer

Admitedly, dataflow frameworks typically offer *much more* functionality than *mess* does. *mess* only deals with calling functions and producing values. *mess* is **not thread-safe**, it does not even know what a thread is! If you need thread-safety, take care of it within the functions that need it.

The goal of *mess* is to provide dataflow functionality, and only this, without compromising **performance**, **readability** and **type-safety**. Of course, you need to pay something to get anything. Here is the cost of *mess*:

* Compilation time: there is some amount of meta-programming involved in *mess*, but not that much. Still, this slows down compilation.
* Static structure: with *mess*, you cannot add or remove subscribers or callbacks on-the-fly. *mess* only lets you define the **static** structure of you program. I find this to be totally acceptable: a program always has a basic static structure. Some programs have a dynamic structure *on top of the static structure*. *mess* takes case of the static part and *let's you build the dynamic part* if you need it, anyway you like. That way, you only pay the cost of a dynamic framework for those parts of your program that benefit from the added flexibiity.

# Acknowledgment
Thanks to all contributors, issue raisers and stargazers!
Most cmake code comes from this repo: https://github.com/bsamseth/cpp-project and Craig Scott's CppCon 2019 talk: Deep CMake for Library Authors. Many thanks to the authors!