/**
 * @file tests.cpp
 * @author L.-C. C.
 * @brief
 * @version 0.1
 * @date 2020-10-29
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <doctest/doctest.h>

#include <mess/mess.hpp>

mess::inline_scheduler_t scheduler = mess::inline_scheduler;

template <typename Value>
struct opaque
{
  Value value;
  bool operator==(const opaque &other) const { return value == other.value; }
};
template <typename Value>
std::ostream &operator<<(std::ostream &os, const opaque<Value> &v)
{
  os << v.value;
  return os;
}

template <typename Return, typename... Args>
opaque<Return> func(opaque<Args>... args)
{
  const auto sum = (args.value + ... + sizeof...(args)) + 1;
  return {static_cast<Return>(sum)};
}

auto one_node =
    mess::make_graph(mess::make_node<mess::arg_predecessors<>,
                                     mess::other_predecessors<>,
                                     mess::successors<>>(func<uint>));

auto two_node_arg =
    mess::make_graph(mess::make_node<mess::arg_predecessors<>,
                                     mess::other_predecessors<>,
                                     mess::successors<1>>(func<uint>),
                     mess::make_node<mess::arg_predecessors<0>,
                                     mess::other_predecessors<>,
                                     mess::successors<>>(func<float, uint>));

auto two_node_other =
    mess::make_graph(mess::make_node<mess::arg_predecessors<>,
                                     mess::other_predecessors<>,
                                     mess::successors<1>>(func<uint>),
                     mess::make_node<mess::arg_predecessors<>,
                                     mess::other_predecessors<0>,
                                     mess::successors<>>(func<float>));

auto two_node_unordered =
    mess::make_graph(mess::make_node<mess::arg_predecessors<1>,
                                     mess::other_predecessors<>,
                                     mess::successors<>>(func<uint, float>),
                     mess::make_node<mess::arg_predecessors<>,
                                     mess::other_predecessors<>,
                                     mess::successors<0>>(func<float>));

auto v = mess::make_graph(
    mess::make_node<mess::arg_predecessors<>,
                    mess::other_predecessors<>,
                    mess::successors<2>>(func<uint>),
    mess::make_node<mess::arg_predecessors<>,
                    mess::other_predecessors<>,
                    mess::successors<2>>(func<float>),
    mess::make_node<mess::arg_predecessors<0, 1>,
                    mess::other_predecessors<>,
                    mess::successors<>>(func<int, uint, float>));

auto diamond =
    mess::make_graph(
        // node 0
        mess::make_node<mess::arg_predecessors<>,
                        mess::other_predecessors<>,
                        mess::successors<1, 2>>(func<uint>),
        // node 1
        mess::make_node<mess::arg_predecessors<0>,
                        mess::other_predecessors<>,
                        mess::successors<3>>(func<float, uint>),
        // node 2
        mess::make_node<mess::arg_predecessors<>,
                        mess::other_predecessors<0>,
                        mess::successors<3>>(func<int>),
        // node 3
        mess::make_node<mess::arg_predecessors<2>,
                        mess::other_predecessors<0>,
                        mess::successors<>>(func<int, int>));

TEST_CASE("One node")
{
  mess::frame_type frame(scheduler, one_node);
  mess::run(frame);
  REQUIRE(std::get<0>(frame._runtime).result.has_value());
  CHECK_EQ(*std::get<0>(frame._runtime).result, opaque<uint>{1});
}

TEST_CASE("Arg")
{
  mess::frame_type frame(scheduler, two_node_arg);
  mess::run(frame);
  REQUIRE(std::get<0>(frame._runtime).result.has_value());
  CHECK_EQ(*std::get<0>(frame._runtime).result, opaque<uint>{1});
  REQUIRE(std::get<1>(frame._runtime).result.has_value());
  CHECK_EQ(*std::get<1>(frame._runtime).result, opaque<float>{3.f});
}

TEST_CASE("Other")
{
  mess::frame_type frame(scheduler, two_node_other);
  mess::run(frame);
  REQUIRE(std::get<0>(frame._runtime).result.has_value());
  CHECK_EQ(*std::get<0>(frame._runtime).result, opaque<uint>{1});
  REQUIRE(std::get<1>(frame._runtime).result.has_value());
  CHECK_EQ(*std::get<1>(frame._runtime).result, opaque<float>{1.f});
}

TEST_CASE("Unordered")
{
  mess::frame_type frame(scheduler, two_node_unordered);
  mess::run(frame);
  REQUIRE(std::get<0>(frame._runtime).result.has_value());
  CHECK_EQ(*std::get<0>(frame._runtime).result, opaque<uint>{3});
  REQUIRE(std::get<1>(frame._runtime).result.has_value());
  CHECK_EQ(*std::get<1>(frame._runtime).result, opaque<float>{1.f});
}

TEST_CASE("V graph")
{
  mess::frame_type frame(scheduler, v);
  mess::run(frame);
  REQUIRE(std::get<0>(frame._runtime).result.has_value());
  CHECK_EQ(*std::get<0>(frame._runtime).result, opaque<uint>{1});
  REQUIRE(std::get<1>(frame._runtime).result.has_value());
  CHECK_EQ(*std::get<1>(frame._runtime).result, opaque<float>{1.f});
  REQUIRE(std::get<2>(frame._runtime).result.has_value());
  CHECK_EQ(*std::get<2>(frame._runtime).result, opaque<int>{5});
}

TEST_CASE("diamond gframe._runtimeraph")
{
  mess::frame_type frame(scheduler, diamond);
  mess::run(frame);
  REQUIRE(std::get<0>(frame._runtime).result.has_value());
  CHECK_EQ(*std::get<0>(frame._runtime).result, opaque<uint>{1});
  REQUIRE(std::get<1>(frame._runtime).result.has_value());
  CHECK_EQ(*std::get<1>(frame._runtime).result, opaque<float>{3.f});
  REQUIRE(std::get<2>(frame._runtime).result.has_value());
  CHECK_EQ(*std::get<2>(frame._runtime).result, opaque<int>{1});
  REQUIRE(std::get<3>(frame._runtime).result.has_value());
  CHECK_EQ(*std::get<3>(frame._runtime).result, opaque<int>{3});
}
