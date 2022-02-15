/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "mess/mess.hpp"
#include "mess/executors/std_thread.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

double func_0()
{
    static int sum = 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(sum * 200));
    std::cout << sum << std::endl;
    return sum;
}
float func_1(int in)
{
    const int sum = in + 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(sum * 200));
    std::cout << sum << std::endl;
    return sum;
}
char func_2(int lhs, int rhs)
{
    const int sum = lhs + rhs + 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(sum * 200));
    std::cout << sum << std::endl;
    return sum;
}

//     0
//     |
//     1   6
//    / \ /
//   2   3
//    \ /
//     4
//     | \
//     5  7
//
//  [clean up]

int main()
{
    auto graph = mess::make_graph(mess::make_node<mess::arg_predecessors<>, mess::other_predecessors<>, mess::successors<1>>(func_0),
                                  mess::make_node<mess::arg_predecessors<0>, mess::other_predecessors<>, mess::successors<2, 3>>(func_1),
                                  mess::make_node<mess::arg_predecessors<1>, mess::other_predecessors<>, mess::successors<4>>(func_1),
                                  mess::make_node<mess::arg_predecessors<1, 6>, mess::other_predecessors<>, mess::successors<4>>(func_2),
                                  mess::make_node<mess::arg_predecessors<2, 3>, mess::other_predecessors<>, mess::successors<5, 7>>(func_2),
                                  mess::make_node<mess::arg_predecessors<4>, mess::other_predecessors<>, mess::successors<>>(func_1),
                                  mess::make_node<mess::arg_predecessors<>, mess::other_predecessors<>, mess::successors<3>>(func_0),
                                  mess::make_node<mess::arg_predecessors<4>, mess::other_predecessors<>, mess::successors<>>(func_1));
    // auto graph = mess::make_graph(mess::make_node<mess::arg_predecessors<>, mess::other_predecessors<>, mess::successors<>>(func_0));

    using executor_type = mess::std_thread_executor;
    using frame_type = mess::frame_type<executor_type, decltype(graph)>;
    executor_type executor;
    frame_type frame(executor, graph);

    mess::run(frame);
    executor.join();
    mess::run(std::make_unique<frame_type>(executor, graph));
}

// sort indexes in order of execution // remove input successors? probably not
// if throw, catch and quit, but clean-up must be run // executor can tell if it was stopped
// replace executor by scheduler
// rerun to reuse frame ?
// comment stuff ? better sort between details or not ? try to hide stuff more.
// frame should be mostly private, and only allow access to result (then friend every function that needs access), that would be neat!
// if all successors only have one predecessor, do not store the result and feed it directly ?
// test the hell out of this // check for constexpr stuff // check inline executor actually is like sequential code
// Set CI back up