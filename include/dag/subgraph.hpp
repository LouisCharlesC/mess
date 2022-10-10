// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "graph.hpp"
#include "successors.hpp"
#include "tag_index.hpp"
#include <set/disjunction.hpp>

#include <tuple>
#include <type_traits>

namespace dag
{
namespace details
{
template <Graph graph, Node node>
constexpr bool is_leaf = std::tuple_size_v<typename dag::successors<graph, typename node::tag>> == 0;

template <typename> struct leafs;
template <template <typename...> typename graph, Node... nodes>
requires Graph<graph<nodes...>>
struct leafs<graph<nodes...>>
{
    using type = set::disjunction<
        std::conditional_t<is_leaf<graph<nodes...>, nodes>, set::types<typename nodes::tag>, set::types<>>...>;
};

} // namespace details

template <Graph graph, set::Types tags> using subgraph_down = typename details::leafs<graph>::type;
template <Graph graph, set::Types tags> using subgraph_up = typename details::leafs<graph>::type;
template <Graph graph, set::Types tags> using subgraph_out = typename details::leafs<graph>::type;
} // namespace dag
