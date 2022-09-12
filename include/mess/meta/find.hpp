// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <mess/meta/sequences.hpp>

#include <cstdint>
#include <tuple>

namespace mess
{
namespace details
{
template <typename tag> struct tag_index
{
    constexpr tag_index(std::size_t index) : index(index){};
    std::size_t index;
};

template <typename flat_graph, std::size_t... all_indexes>
consteval auto make_index_map(std::index_sequence<all_indexes...>)
{
    return std::tuple<tag_index<typename std::tuple_element_t<all_indexes, flat_graph>::tag>...>(all_indexes...);
}

template <typename flat_graph>
constexpr auto index_map = make_index_map<flat_graph>(std::make_index_sequence<std::tuple_size_v<flat_graph>>());
} // namespace details

template <typename flat_graph, typename tag>
constexpr std::size_t find = std::get<details::tag_index<tag>>(details::index_map<flat_graph>).index;

namespace details
{
template <typename flat_graph, typename... all_tags> constexpr auto to_indexes(tags<all_tags...>)
{
    return indexes<mess::find<flat_graph, all_tags>...>();
}
} // namespace details

template <typename flat_graph, typename tags> using to_indexes = decltype(details::to_indexes<flat_graph>(tags()));

template <typename flat_graph, typename tag>
using graph_node_t = std::tuple_element_t<find<flat_graph, tag>, flat_graph>;
} // namespace mess
