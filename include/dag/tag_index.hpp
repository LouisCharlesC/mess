// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "graph.hpp"
#include "tags.hpp"
#include <set/contains.hpp>

#include <cstddef>
#include <tuple>

namespace dag
{
namespace details
{
template <typename tag> struct tag_index
{
    constexpr tag_index(std::size_t index) : index(index){};
    std::size_t index;
};

template <Graph graph, std::size_t... indexes> consteval auto make_index_map(std::index_sequence<indexes...>)
{
    return std::tuple<tag_index<typename std::tuple_element_t<indexes, graph>::tag>...>(indexes...);
}

template <Graph graph>
constexpr auto index_map = make_index_map<graph>(std::make_index_sequence<std::tuple_size_v<graph>>());
} // namespace details

template <Graph graph, typename tag>
requires set::contains<tag, dag::tags<graph>>
constexpr std::size_t index_of = std::get<details::tag_index<tag>>(details::index_map<graph>).index;

// namespace details
// {
// template <typename flat_graph, typename... all_tags> constexpr auto to_indexes(tags<all_tags...>)
// {
//     return indexes<mess::find<flat_graph, all_tags>...>();
// }
// } // namespace details

// template <typename flat_graph, typename tags> using to_indexes = decltype(details::to_indexes<flat_graph>(tags()));

template <typename graph, typename tag> using graph_node = std::tuple_element_t<index_of<graph, tag>, graph>;
} // namespace dag
