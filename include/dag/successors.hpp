// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "graph.hpp"
#include "tag_index.hpp"
#include <set/concatenate.hpp>
#include <set/contains.hpp>
#include <set/types.hpp>

#include <cstddef>
#include <tuple>

namespace dag
{
namespace details
{
template <Graph graph, typename tag> struct successors;
template <typename tag, template <typename...> typename graph, Node... nodes>
requires Graph<graph<nodes...>>
struct successors<graph<nodes...>, tag>
{
    using type = set::concatenate<std::conditional_t<
        set::contains<typename dag::graph_node<graph<nodes...>, tag>::tag, typename nodes::predecessors>,
        set::types<typename nodes::tag>, set::types<>>...>;
};
} // namespace details

template <Graph graph, typename tag> using successors = typename details::successors<graph, tag>::type;
} // namespace dag
