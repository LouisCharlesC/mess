// Copyright(c) 2022-2023 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "graph.hpp"
#include <set/index_of.hpp>

namespace dag
{
template <std::size_t index_, auto predecessors> struct index_node
{
    static constexpr std::size_t index = index_;
};

// sort: until done, add all ready in order + remove them from all preds

// Base template, fails to compile if the specialization is not triggered.
template <Graph graph> constexpr struct fail to_index_graph;
// Specialization to access the nodes.
template <typename... nodes> constexpr auto to_index_graph<graph types::set<nodes...>> = std::tuple<>;
} // namespace dag