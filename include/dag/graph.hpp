// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <set/types.hpp>

#include <cassert>
#include <tuple>
#include <utility>

namespace dag
{
template <typename tag_t, typename... predecessors_t> struct node
{
    static_assert(!(std::is_same_v<tag_t, predecessors_t> || ...), "tag_t must not appear in predecessors_t...");

    using tag = tag_t;
    using predecessors = set::types<predecessors_t...>;
};

template <typename T>
concept Node = requires(T)
{
    typename T::tag;
    typename T::predecessors;
};

template <Node... nodes> using graph = set::types<nodes...>;

template <typename nodes> constexpr bool is_graph = false;
template <Node... nodes> constexpr bool is_graph<graph<nodes...>> = true;

template <typename T>
concept Graph = is_graph<T>;
} // namespace dag