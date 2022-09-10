// Copyright(c) 2019-2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

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

// Functions with exotic return types also work. This one returns the std::endl manupulator, which is a function
// pointer.
using ManipulatorFnPtr =
    std::basic_ostream<char, std::char_traits<char>> &(*)(std::basic_ostream<char, std::char_traits<char>> &);
ManipulatorFnPtr std_endl()
{
    return std::endl;
}

// Template lambda functions too.
auto stream = [](std::basic_ostream<char, std::char_traits<char>> &os,
                 auto val) -> std::basic_ostream<char, std::char_traits<char>> & { return os << val; };
} // namespace

int main()
{
    // Each node in the graph is identified by a type.
    struct One;
    struct Two;
    struct Three;
    struct Four;
    struct Five;

    // The graph is built here.
    //
    // 1: std_cout   2: hello_world
    //          \     /
    //         4: stream  3: std_endl
    //               \     /
    //              5: stream
    auto print_hello_world = mess::make_graph(
        // Node 0
        mess::make_node<One, mess::arg_predecessors<>, mess::successors<Four>>(std_cout),
        mess::make_node<Two, mess::arg_predecessors<>, mess::successors<Four>>(hello_world),
        mess::make_node<Three, mess::arg_predecessors<>, mess::successors<Five>>(std_endl),
        mess::make_node<Four, mess::arg_predecessors<One, Two>, mess::successors<Five>>(stream),
        mess::make_node<Five, mess::arg_predecessors<Four, Three>>(stream));

    // The inline scheduler simply invokes the functions it is given.
    // Replace the next line with "mess::std_thread_scheduler scheduler;" (and don't forget to #include
    // <mess/schedulers/std_thread.hpp>) to get a parallel execution of the graph, where each function is invoked in a
    // separate thread.
    mess::inline_scheduler_t scheduler;
    // Execute the graph using the specified scheduler.
    mess::run(mess::frame_type(scheduler, print_hello_world));
}
