// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "graph.hpp"
#include <set/types.hpp>

namespace dag
{
namespace details
{
struct collect;
struct ordered;
struct filter;
struct invoke_result;
struct invoke;

template <Graph graph, typename visitor, set::Types visit_from> using visit = typename details::tags<graph>::type;

// template <typename> struct tags;
// template <template <typename...> typename graph, Node... nodes>
// requires Graph<graph<nodes...>>
// struct tags<graph<nodes...>>
// {
//     using type = set::types<typename nodes::tag...>;
// };
} // namespace details

template <Graph graph, set::Types visit_from, typename visitor> using visit = typename details::visit<graph>::type;
} // namespace dag
