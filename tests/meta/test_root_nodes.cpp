// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <mess/graph.hpp>
#include <mess/meta/root_nodes.hpp>

#include <cassert>
#include <type_traits>

namespace
{
struct First;
struct Second;
struct Last;

template <typename Return, typename... Args> Return func(Args...)
{
    return {};
}
} // namespace

constexpr auto three_nodes =
    mess::make_graph(mess::make_node<First, mess::arg_predecessors<Last, Second>>(func<int>),
                     mess::make_node<Second, mess::arg_predecessors<>, mess::successors<First>>(func<int>),
                     mess::make_node<Last, mess::arg_predecessors<>>(func<int>));
using three_nodes_t = decltype(three_nodes);

static_assert(std::is_same_v<decltype(mess::root_indexes<three_nodes_t>()), mess::indexes<1, 2>>, "");