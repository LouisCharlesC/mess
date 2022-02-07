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

#include <array>
#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

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
    ::mess::flat_graph graph;
    std::get<0>(graph) = mess::node{func,
                                    {},
                                    0,
                                    {1}};
    std::get<1>(graph) = mess::node{func,
                                    {0},
                                    1,
                                    {2, 3}};
    std::get<2>(graph) = mess::node{func,
                                    {1},
                                    1,
                                    {4}};
    std::get<3>(graph) = mess::node{func,
                                    {1},
                                    1,
                                    {4}};
    std::get<4>(graph) = mess::node{func,
                                    {2, 3},
                                    2,
                                    {5}};

    std::atomic_flag signal;
    std::get<5>(graph) = mess::node{[&signal](int, int) -> int
                                    {
                                        signal.test_and_set();
                                        signal.notify_all();
                                        return 0;
                                    },
                                    {4},
                                    1,
                                    {}};
    mess::executor rt;
    mess::run_and_take_care_of_deleting_the_frame(std::make_unique<mess::frame>(mess::make_frame(graph, rt)));
    signal.wait(false);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // descriptor[5] = mess::node{func,
    //                             {4},
    //                             1,
    //                             {}};
    // mess::run(descriptor, rt);
}

// constexpr number of entries
//   std::tuple<entry...>
// customize on runtime, think about the kit
//    kit: execute, countdownlatch/isready
// constexpr arguments index
// remove std::function, shared_ptr to unique_ptr
// constexpr successors index
// atomic latch, maybe ?
// allow providing a function to call last, I guess then run() should return whatever must stay alive for the function to be called after all else is deleted