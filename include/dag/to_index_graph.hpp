// Copyright(c) 2022-2023 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "graph.hpp"
#include <set/index_of.hpp>

namespace dag
{
namespace indexes
{
template <std::size_t index, std::size_t... predecessors> constexpr auto node = ::indexes::set<index, predecessors...>;

template <auto... nodes> constexpr auto graph = types::make<nodes...>;

// sort: until done, add all ready in order + remove them from all preds

} // namespace indexes

// Base template, fails to compile if the specialization is not triggered.
template <types::Set tags, Node node> constexpr struct fail to_index_node;
// Specialization to access the nodes.
template <types::Set tags, typename tag, typename... predecessors>
constexpr auto to_index_node<tags, node<tag(predecessors...)>> =
    indexes::node<types::index_of<tags, tag>, types::index_of<tags, predecessors>...>;

// Base template, fails to compile if the specialization is not triggered.
template <Graph graph> constexpr struct fail to_index_graph;
// Specialization to access the nodes.
template <Node... nodes>
constexpr auto to_index_graph<types::set<nodes...>> = indexes::graph<to_index_node<tags<nodes...>, nodes>...>;
} // namespace dag