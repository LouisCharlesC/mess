/**
 * @file helloworld.cpp
 * @author L.-C. C.
 * @brief
 * @version 0.1
 * @date 2019-04-19
 *
 * @copyright Copyright (c) 2019
 *
 */

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

    // Functions with exotic return types (this one returns a function pointer) also work.
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
    // The graph is built here.
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
    mess::inline_scheduler scheduler;
    // Execute the graph using the specified scheduler.
    mess::run(mess::frame_type(scheduler, print_hello_world));
    // This is a no-op when using mess::inline_scheduler.
    scheduler.join();
}
