// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <dag/graph.hpp>

namespace datapath
{
template <typename invocable_type_t, typename signature> struct node;
template <typename invocable_type_t, typename tag_t, typename... predecessors_t>
struct node<invocable_type_t, tag_t(predecessors_t...)> : dag::node<tag_t(predecessors_t...)>
{
    using invocable_type = invocable_type_t;

    invocable_type invocable;
};

// template <typename flat_graph, std::size_t index>
// using arg_predecessor_indexes =
//     to_indexes<flat_graph, typename std::tuple_element_t<index, flat_graph>::arg_predecessor_tags>;
// template <typename flat_graph, std::size_t index>
// using unordered_predecessor_indexes =
//     disjunction<arg_predecessor_indexes<flat_graph, index>,
//                 to_indexes<flat_graph, typename std::tuple_element_t<index, flat_graph>::other_predecessor_tags>>;
// template <typename flat_graph, std::size_t index>
// using successor_indexes = to_indexes<flat_graph, typename std::tuple_element_t<index, flat_graph>::successor_tags>;

template <typename signature, typename invocable_type>
constexpr node<invocable_type, signature> make_node(invocable_type &&invocable)
{
    return {.invocable = std::forward<invocable_type>(invocable)};
}

template <dag::Node... nodes_type> constexpr dag::graph<nodes_type...> make_graph(nodes_type &&...nodes)
{
    return {std::forward<nodes_type>(nodes)...};
}
} // namespace datapath
