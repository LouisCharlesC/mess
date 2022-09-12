# *Software is a mess, embrace it !*
*mess* is a compile-time, C++20 library for dataflow programing that lets you build an execution graph and independently decide how it will be executed (sequentially, using a thread-pool, etc.).  
Tons of such frameworks exist, but *mess* is meant to be non-intrusive and as low-overhead as possible (see section [Hello world](#Hello-world) for a zero runtime overhead demo).

## Briefly
*mess* lets you build an execution graph by listing the functions that need to be executed and the dependencies that link them. This forms your program's dataflow, and *mess* can execute it. The same graph can be executed several times and with different execution strategies. Two strategies are provided by the library: sequential and using one `std::thread` per function. The sequential strategy is equivalent to plain function calls, whereas the `std::thread` strategy uses as much parallelism as the graph allows. This lets you choose, for the same execution graph, the best execution strategy given the context. For instance, run sequentially first to facilitate debugging, or run sequentially for small inputs and in parallel for large ones. Your own execution strategies (e.g. a thread-pool) can easily be plugged into *mess*.

As an example, here is how to build and execute a diamond graph:

```c++
//   1
//  / \
// 2   3
//  \ /
//   4
struct One;   // 1
struct Two;   // 1
struct Three; // 1
struct Four;  // 1
auto diamond =                                                                          // 1
    mess::make_graph(                                                                   // 1
        mess::make_node<One,
                        mess::arg_predecessors<>,                                       // 1
						mess::successors<Two, Three>>
                        ([](){return 0;}),                       // 1
        mess::make_node<Two, mess::arg_predecessors<One>,
                        mess::successors<Four>>([](int i){return i+1;}),   // 1
        mess::make_node<Three, mess::arg_predecessors<One>,
                        mess::successors<Four>>([](int i){return i+2;}), // 1 
        mess::make_node<Four, mess::arg_predecessors<Two, Three>>
                        ([](int i, int j){return i+j;}));  // 1

mess::frame_type frame(mess::inline_scheduler, diamond);                                // 2
mess::run(frame);                                                                       // 3
```

Firstly (1), the graph is built. Four nodes are created by specifying their `arg_predecessors`, `other_predecessors`, `successors` and the function to invoke. The ouput of each node listed in `arg_predecessors` is passed to the current node's function. `other_predecessors` indicate nodes that should be executed before the current node, although they do not provide an argument to the node's function. `successors` lists the nodes that have the current node as a predecessor. The `successors` list is redundant, and should be computed automatically in the future.

Then (2), a frame is created to hold all the data needed for *mess* to execute the graph using the inline scheduler.

Thirdly (3), the graph is executed.

## Parallel execution
To execute the same graph in parallel (with nodes 1 and 2 being run at the same), simply use *mess*' `std_thread_scheduler` like so:

```c++
mess::std_thread_scheduler scheduler;       // 1
mess::frame_type frame(scheduler, diamond); // 2
mess::run(frame);                           // 3
```

Here, a `std_thread_scheduler` is first (1) instantiated.

The rest (2, 3 and 5) is almost like before. Only (4) differs because we have to wait for the execution to finish before inspecting the result.

## Fire-and-forget
If the graph is executed in a different thread (like when using `std_thread_scheduler`) and you do not care about the result, you can launch the execution and let *mess* clean up behind you:

```c++
mess::std_thread_scheduler scheduler;
using frame_type = mess::frame_type<mess::std_thread_scheduler, decltype(diamond)>;
mess::run(std::make_unique<frame_type>(scheduler, diamond));
```

Here, a `frame` is created and directly passed to the `run` function. This causes *mess* to take care of having the frame live as long as needed, and deleting it after the graph is executed.

Be aware that `std_thread_scheduler` will `join()` on destruction. 

## WIP

*mess* is currently under development. Version 1.0, as a bare minimum, must:

1. Correctly handle exceptions thrown by a node's function, and cancel nodes that cannot be executed anymore.

It is foreseen that future versions might:

1. Allow constexpr execution
1. Automatically compute the `successors` list.
1. Provide nice compilation errors rather than the typical template instantiation error messages.

Another neat way of improving *mess* would be to have a nicer way to build the graph. This could easily be built on top of *mess* to simplify its use.

## Hello world

Here is *mess*'s “Hello, world!”. With optimizations enabled, this code compiles to the same executable as a plain C++ “Hello, world!” (shown below). This is verified in the tests.  

```c++
#include <mess/mess.hpp>

#include <iostream>

// We will break the hello world program into this execution graph.
// Each node in this graph is a function. "stream" is the streaming operator<<.
//
// std_cout   hello_world
//        \    /
//        stream    std_endl
//             \    /
//             stream

// The functions we need are defined here.
namespace
{
    // Just a function returning std::cout.
    std::basic_ostream<char, std::char_traits<char>> &std_cout()
    {
        return std::cout;
    }

    // Another simple function
    const char *hello_world()
    {
        return "Hello, World!";
    }

    // Functions with exotic return types also work. This one returns the std::endl manupulator, which is a function pointer.
    using ManipulatorFnPtr = std::basic_ostream<char, std::char_traits<char>> &(*)(std::basic_ostream<char, std::char_traits<char>> &);
    ManipulatorFnPtr std_endl()
    {
        return std::endl;
    }

    // Template lambda functions too.
    auto stream = [](std::basic_ostream<char, std::char_traits<char>> &os, auto val) -> std::basic_ostream<char, std::char_traits<char>> &
    { return os << val; };
} // namespace

int main()
{
    // The graph is built here. As a reminder:
	//
	// std_cout   hello_world
	//        \    /
	//        stream    std_endl
	//             \    /
	//             stream
    auto print_hello_world = mess::make_graph(
        // Node 0
        mess::make_node<mess::arg_predecessors<>, mess::other_predecessors<>,
                        mess::successors<3>>(std_cout),
        // Node 1
        mess::make_node<mess::arg_predecessors<>, mess::other_predecessors<>,
                        mess::successors<3>>(hello_world),
        // Node 2
        mess::make_node<mess::arg_predecessors<>, mess::other_predecessors<>,
                        mess::successors<4>>(std_endl),
        // Node 3
        mess::make_node<mess::arg_predecessors<0, 1>, mess::other_predecessors<>,
                        mess::successors<4>>(stream),
        // Node 4
        mess::make_node<mess::arg_predecessors<3, 2>>(stream));

    // The inline scheduler simply invokes the functions it is given.
    // Replace the next line with "mess::std_thread_scheduler scheduler;" (and don't forget to #include <mess/schedulers/std_thread.hpp>)
    // to get a parallel execution of the graph, where each function is invoked in a separate thread.
    mess::inline_scheduler_type scheduler;
    // Execute the graph using the specified scheduler.
    mess::run(mess::frame_type(scheduler, print_hello_world));
    // This is a no-op when using mess::inline_scheduler.
    scheduler.join();
}
```

Here is the plain “Hello, world!” the above example compiles equal to:

```c++
#include <iostream>
int main()
{
	std::cout << "Hello, World!" << std::endl;
	return 0;
}
```

## Why use *mess*

### What *mess* saves you

All messaging frameworks I know of have a runtime cost:

* Memory cost: to store callable objects (function pointers, std::function instances, etc.) used to store the subscription callbacks.
* Processing cost: to iterate through the callbacks.
* Indirect calls cost: to invoke callbacks through function pointers, std::function or the framework's virtual methods.
* Conversion cost: to convert your business domain type into the type the framework forces you to use to exchange data.
* Thread-safety cost: to safely manage the dynamic messaging structure during execution.
* Optimization cost: for any level of indirection or polymorphism that prevent the optimizer from inlining and reasonning about the code.
* Run-time error detection cost: when polymorphism prevents type errors to be detected at compile time.
* etc.

*mess* aims to have as little runtime cost as possible. The graph is built as a compile-time type, no thread safe mechanism is used unless the scheduler requires it, and function arguments are stored as-is (in a `std::optional`).

### What *mess* does not offer

Admitedly, other frameworks offer *much more* functionality than *mess* does. The goal of *mess* is to provide dataflow , and only this, without compromising **performance**, **readability** and **type-safety**. Of course, you need to pay something to get anything. Here is the cost of *mess*:

* Compilation time: there is some amount of meta-programming involved in *mess*, but not that much. Still, this slows down compilation.
* Static structure: with *mess*, you cannot change the number of nodes at runtime. *mess* only lets you define the **static** structure of you program. I find this to be totally acceptable: a program always has a basic static structure. Some programs have a dynamic structure *on top of the static structure*. *mess* takes case of the static part and *let's you build the dynamic part* if you need it, anyway you like. That way, you only pay the cost of a dynamic framework for those parts of your program that benefit from the added flexibility.

# Acknowledgment
Thanks to all contributors, issue raisers and stargazers!
Most cmake code comes from this repo: https://github.com/bsamseth/cpp-project and Craig Scott's CppCon 2019 talk: Deep CMake for Library Authors. Many thanks to the authors!