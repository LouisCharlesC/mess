// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <datapath/graph.hpp>
#include <doctest/doctest.h>
#include <func.hpp>

namespace
{

struct top;
struct left;
struct right;
struct bottom;

// auto two_node_arg = mess::make_graph(
//     mess::make_node<top, mess::arg_predecessors<>, mess::other_predecessors<>, mess::successors<1>>(func<uint>),
//     mess::make_node<bottom, mess::arg_predecessors<0>, mess::other_predecessors<>, mess::successors<>>(
//         func<float, uint>));

// auto two_node_other = mess::make_graph(
//     mess::make_node<top, mess::arg_predecessors<>, mess::other_predecessors<>, mess::successors<1>>(func<uint>),
//     mess::make_node<bottom, mess::arg_predecessors<>, mess::other_predecessors<0>, mess::successors<>>(func<float>));

// auto two_node_unordered = mess::make_graph(
//     mess::make_node<bottom, mess::arg_predecessors<1>, mess::other_predecessors<>, mess::successors<>>(
//         func<uint, float>),
//     mess::make_node<top, mess::arg_predecessors<>, mess::other_predecessors<>, mess::successors<0>>(func<float>));

// auto v = mess::make_graph(
//     mess::make_node<left, mess::arg_predecessors<>, mess::other_predecessors<>, mess::successors<2>>(func<uint>),
//     mess::make_node<right, mess::arg_predecessors<>, mess::other_predecessors<>, mess::successors<2>>(func<float>),
//     mess::make_node<bottom, mess::arg_predecessors<0, 1>, mess::other_predecessors<>, mess::successors<>>(
//         func<int, uint, float>));

// auto diamond = mess::make_graph(
//     mess::make_node<top, mess::arg_predecessors<>, mess::other_predecessors<>, mess::successors<1, 2>>(func<uint>),
//     mess::make_node<left, mess::arg_predecessors<0>, mess::other_predecessors<>, mess::successors<3>>(
//         func<float, uint>),
//     mess::make_node<right, mess::arg_predecessors<>, mess::other_predecessors<0>, mess::successors<3>>(func<int>),
//     mess::make_node<bottom, mess::arg_predecessors<2>, mess::other_predecessors<0>, mess::successors<>>(
//         func<int, int>));
} // namespace

TEST_CASE("One node")
{
    auto one_node = datapath::make_graph(datapath::make_node<top>(func<int>));
    // dag::graph<top, mess::arg_predecessors<>, mess::other_predecessors<>, mess::successors<>>(func<uint>));
}

// TEST_CASE("Arg")
// {
//     mess::frame_type frame(scheduler, two_node_arg);
//     mess::run(frame);
//     REQUIRE(std::get<0>(frame._runtime).result.has_value());
//     CHECK_EQ(*std::get<0>(frame._runtime).result, opaque<uint>{1});
//     REQUIRE(std::get<1>(frame._runtime).result.has_value());
//     CHECK_EQ(*std::get<1>(frame._runtime).result, opaque<float>{3.f});
// }

// TEST_CASE("Other")
// {
//     mess::frame_type frame(scheduler, two_node_other);
//     mess::run(frame);
//     REQUIRE(std::get<0>(frame._runtime).result.has_value());
//     CHECK_EQ(*std::get<0>(frame._runtime).result, opaque<uint>{1});
//     REQUIRE(std::get<1>(frame._runtime).result.has_value());
//     CHECK_EQ(*std::get<1>(frame._runtime).result, opaque<float>{1.f});
// }

// TEST_CASE("Unordered")
// {
//     mess::frame_type frame(scheduler, two_node_unordered);
//     mess::run(frame);
//     REQUIRE(std::get<0>(frame._runtime).result.has_value());
//     CHECK_EQ(*std::get<0>(frame._runtime).result, opaque<uint>{3});
//     REQUIRE(std::get<1>(frame._runtime).result.has_value());
//     CHECK_EQ(*std::get<1>(frame._runtime).result, opaque<float>{1.f});
// }

// TEST_CASE("V graph")
// {
//     mess::frame_type frame(scheduler, v);
//     mess::run(frame);
//     REQUIRE(std::get<0>(frame._runtime).result.has_value());
//     CHECK_EQ(*std::get<0>(frame._runtime).result, opaque<uint>{1});
//     REQUIRE(std::get<1>(frame._runtime).result.has_value());
//     CHECK_EQ(*std::get<1>(frame._runtime).result, opaque<float>{1.f});
//     REQUIRE(std::get<2>(frame._runtime).result.has_value());
//     CHECK_EQ(*std::get<2>(frame._runtime).result, opaque<int>{5});
// }

// TEST_CASE("diamond gframe._runtimeraph")
// {
//     mess::frame_type frame(scheduler, diamond);
//     mess::run(frame);
//     REQUIRE(std::get<0>(frame._runtime).result.has_value());
//     CHECK_EQ(*std::get<0>(frame._runtime).result, opaque<uint>{1});
//     REQUIRE(std::get<1>(frame._runtime).result.has_value());
//     CHECK_EQ(*std::get<1>(frame._runtime).result, opaque<float>{3.f});
//     REQUIRE(std::get<2>(frame._runtime).result.has_value());
//     CHECK_EQ(*std::get<2>(frame._runtime).result, opaque<int>{1});
//     REQUIRE(std::get<3>(frame._runtime).result.has_value());
//     CHECK_EQ(*std::get<3>(frame._runtime).result, opaque<int>{3});
// }
