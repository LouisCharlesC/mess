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
#include "mess/schedulers/std_thread.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

template <std::size_t index>
char func(int in)
{
  std::cout << "s " << index << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(in * 200));
  std::cout << "e " << index << ": " << in << std::endl;
  return in;
}

template <std::size_t index>
double func_0()
{
  return func<index>(1);
}
template <std::size_t index>
float func_1(int in)
{
  return func<index>(in + 1);
}
template <std::size_t index>
char func_2(int lhs, int rhs)
{
  return func<index>(lhs + rhs + 1);
}

//     7
//     |
// 6   1
//  \ / \
//   2   3
//    \ /
//     4
//     | \
//     8  0
//        |
//        5
//
//  [clean up]

int main()
{
  auto graph = mess::make_graph(
      mess::make_node<mess::arg_predecessors<4>, mess::other_predecessors<>,
                      mess::successors<5>>(func_1<0>),
      mess::make_node<mess::arg_predecessors<7>, mess::other_predecessors<>,
                      mess::successors<2, 3>>(func_1<1>),
      mess::make_node<mess::arg_predecessors<1, 6>, mess::other_predecessors<>,
                      mess::successors<4>>(func_2<2>),
      mess::make_node<mess::arg_predecessors<1>, mess::other_predecessors<>,
                      mess::successors<4>>(func_1<3>),
      mess::make_node<mess::arg_predecessors<2, 3>, mess::other_predecessors<>,
                      mess::successors<8, 0>>(func_2<4>),
      mess::make_node<mess::arg_predecessors<0>, mess::other_predecessors<>,
                      mess::successors<>>(func_1<5>),
      mess::make_node<mess::arg_predecessors<>, mess::other_predecessors<>,
                      mess::successors<2>>(func_0<6>),
      mess::make_node<mess::arg_predecessors<>, mess::other_predecessors<>,
                      mess::successors<1>>(func_0<7>),
      mess::make_node<mess::arg_predecessors<4>, mess::other_predecessors<>,
                      mess::successors<>>(func_1<8>));
  // auto graph = mess::make_graph(mess::make_node<mess::arg_predecessors<>,
  // mess::other_predecessors<>, mess::successors<>>(func_0));

  // using scheduler_type = mess::inline_scheduler;
  using scheduler_type = mess::std_thread_scheduler;
  using frame_type = mess::frame_type<scheduler_type, decltype(graph)>;
  scheduler_type scheduler;
  frame_type frame(scheduler, graph);

  mess::run(frame);
  scheduler.join();
  mess::run(std::make_unique<frame_type>(scheduler, graph));
}

// will we have to use a meta library to make things easier to understand ?

// Set CI back up
// check for constexpr stuff
// scheduler actually is like inline code if throw, catch and quit, but clean-up must be run, even others branches must
// noexcept everything, if exceptions are handle from the previous task.
// Rerun to reuse frame ?
// comment stuff ? better sort between details or not ? try to hide stuff more.
// frame should be mostly private, and only allow access to result (then friend every function that needs access), that would be neat!
// if all successors only have one predecessor, do not store the result and feed it directly ?
// remove input successors? probably not
// scheduler can tell if it was stopped
