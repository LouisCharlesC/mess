// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "graph.hpp"
#include <set/contains.hpp>
#include <set/indexes.hpp>

#include <algorithm>
#include <cstddef>

namespace dag
{
template <Graph graph, auto graph_indexes, std::size_t index, typename = std::make_index_sequence<graph_indexes.size()>>
requires set::is_index_set<graph_indexes>
constexpr int successors;

template <Graph graph, auto graph_indexes, std::size_t index, std::size_t... Is>
constexpr auto successors<graph, graph_indexes, index, std::index_sequence<Is...>> =
    set::indexes<std::find(std::tuple_element_t<graph_indexes[Is], graph>::predecessors.cbegin(),
                             std::tuple_element_t<graph_indexes[Is], graph>::predecessors.cend(), index));
} // namespace dag
