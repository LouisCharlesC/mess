// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "graph.hpp"
#include "tags.hpp"
#include <set/index_of.hpp>
#include <set/indexes.hpp>
#include <set/types.hpp>

#include <algorithm>
#include <cstddef>
#include <tuple>
#include <utility>

namespace dag
{
namespace details
{
// Base template, with a default parameter that will make a std::index_sequence, triggering the specialization.
template <types::set graph_tags, types::set predecessors_tags,
          typename Is = std::make_index_sequence<std::tuple_size_v<predecessors_tags>>>
struct predecessors;
// Specialization to access the std::index_sequence.
template <types::set graph_tags, types::set predecessors_tags, std::size_t... Is>
struct predecessors<graph_tags, predecessors_tags, std::index_sequence<Is...>>
{
    static constexpr auto indexes =
        set::indexes<set::index_of<graph_tags, std::tuple_element_t<Is, predecessors_tags>>...>;
};
} // namespace details

template <Graph graph, std::size_t index>
requires(index < std::tuple_size_v<graph>)
constexpr auto predecessors =
    details::predecessors<tags<graph>, typename std::tuple_element_t<index, graph>::predecessors>::indexes;
} // namespace dag
