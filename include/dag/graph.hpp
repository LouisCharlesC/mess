// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <mess/meta/concatenate.hpp>
#include <mess/meta/find.hpp>
#include <mess/meta/sequences.hpp>

#include <cassert>
#include <tuple>
#include <utility>

namespace mess
{
template <typename... nodes_type> using flat_graph = std::tuple<nodes_type...>;

template <typename tag_t, typename invocable_type_t, typename arg_predecessors_t, typename successors_t,
          typename other_predecessors_t>
struct node_type
{
    static_assert(is_arg_predecessors_type(arg_predecessors_t()),
                  "mess::node_type's second template argument must be of type mess::args.");
    static_assert(is_other_predecessors_type(other_predecessors_t()),
                  "mess::node_type's third template argument must be of type mess::after.");
    static_assert(is_successors_type(successors_t()),
                  "mess::node_type's fourth template argument must be of type mess::successors.");

    using tag = tag_t;
    using invocable_type = invocable_type_t;
    using arg_predecessor_tags = arg_predecessors_t;
    using other_predecessor_tags = other_predecessors_t;
    using successor_tags = successors_t;

    invocable_type invocable;
};

template <typename flat_graph, std::size_t index>
using arg_predecessor_indexes =
    to_indexes<flat_graph, typename std::tuple_element_t<index, flat_graph>::arg_predecessor_tags>;
template <typename flat_graph, std::size_t index>
using unordered_predecessor_indexes =
    concatenate<arg_predecessor_indexes<flat_graph, index>,
                to_indexes<flat_graph, typename std::tuple_element_t<index, flat_graph>::other_predecessor_tags>>;
template <typename flat_graph, std::size_t index>
using successor_indexes = to_indexes<flat_graph, typename std::tuple_element_t<index, flat_graph>::successor_tags>;

template <typename tag, typename arg_predecessors_tags, typename successors = mess::successors<>,
          typename other_predecessors_tags = mess::other_predecessors<>, typename invocable_type = void>
constexpr auto make_node(invocable_type &&invocable)
{
    return node_type<tag, invocable_type, arg_predecessors_tags, successors, other_predecessors_tags>{
        std::forward<invocable_type>(invocable)};
}

template <typename... nodes_type> constexpr auto make_graph(nodes_type &&...nodes)
{
    return std::make_tuple(std::forward<nodes_type>(nodes)...);
}
} // namespace mess