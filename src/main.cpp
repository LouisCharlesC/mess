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
    std::vector<mess::entry> descriptor(6);

    std::atomic_flag signal;

    descriptor[0] = mess::entry{func,
                                {},
                                0,
                                {1}};
    descriptor[1] = mess::entry{func,
                                {0},
                                1,
                                {2, 3}};
    descriptor[2] = mess::entry{func,
                                {1},
                                1,
                                {4}};
    descriptor[3] = mess::entry{func,
                                {1},
                                1,
                                {4}};
    descriptor[4] = mess::entry{func,
                                {2, 3},
                                2,
                                {5}};

    descriptor[5] = mess::entry{[&signal](int, int) -> int
                                {
                                    signal.test_and_set();
                                    signal.notify_all();
                                    return 0;
                                },
                                {4},
                                1,
                                {}};
    mess::run(std::make_unique<mess::frame>(mess::make_frame(descriptor)));
    signal.wait(false);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // descriptor[5] = mess::entry{func,
    //                             {4},
    //                             1,
    //                             {}};
    // mess::run(descriptor);
}

// constexpr number of entries
// customize on runtime, think about the kit
//    kit: execute, countdownlatch/isready
// constexpr arguments index
// remove std::function, shared_ptr to unique_ptr
// constexpr successors index
// atomic latch, maybe ?