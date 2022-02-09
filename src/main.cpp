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

#include "mess/mess.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

int func()
{
    constexpr int sum = 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(sum * 100));
    std::cout << sum << std::endl;
    return sum;
}
int func(int in)
{
    const int sum = in + 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(sum * 100));
    std::cout << sum << std::endl;
    return sum;
}
int func(int lhs, int rhs)
{
    const int sum = lhs + rhs;
    std::this_thread::sleep_for(std::chrono::milliseconds(sum * 100));
    std::cout << sum << std::endl;
    return sum;
}

//     0
//     |
//     1
//    / \
//   2   3
//    \ /
//     4
//     |
//     5

int main()
{
    ::mess::flat_graph graph = ::mess::flat_graph{mess::node_type<int (&)(), mess::arg_predecessors<>, mess::successors<1>>{func, {}},
                                                  mess::node_type<int (&)(int), mess::arg_predecessors<0>, mess::successors<2, 3>>{func,
                                                                                                                                   {}},
                                                  mess::node_type<int (&)(int), mess::arg_predecessors<1>, mess::successors<4>>{func,
                                                                                                                                {}},
                                                  mess::node_type<int (&)(int), mess::arg_predecessors<1>, mess::successors<4>>{func,
                                                                                                                                {}},
                                                  mess::node_type<int (&)(int, int), mess::arg_predecessors<2, 3>, mess::successors<5>>{func,
                                                                                                                                        {}},
                                                  mess::node_type<int (&)(int), mess::arg_predecessors<4>, mess::successors<>>{func, {}}};

    mess::executor_type rt;

    mess::make_frame_and_run(graph, rt);
    // mess::run_and_take_care_of_deleting_the_frame(std::make_unique<mess::frame_type>(mess::make_frame(graph, rt)));
}

// constexpr arguments index
// remove std::function, shared_ptr to unique_ptr
// atomic latch
// allow providing a function to call last, I guess then run() should return whatever must stay alive for the function to be called after all else is deleted