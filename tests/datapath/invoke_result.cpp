// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

// #include <doctest/doctest.h>

// #include <datapath/graph.hpp>

// namespace
// {
// template <typename Return, typename... Args> constexpr Return func(Args... args)
// {
//     return (args + ... + sizeof...(args)) + 1;
// }

// struct top;
// struct left;
// struct right;
// struct bottom;

// TEST_CASE("One node")
// {
//     auto one_node = datapath::make_graph(datapath::make_node<top>(func<int>));
//     // dag::graph<top, mess::arg_predecessors<>, mess::other_predecessors<>, mess::successors<>>(func<uint>));
// }
