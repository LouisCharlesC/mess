// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "concatenate.hpp"
#include "contains.hpp"
#include "find.hpp"
#include "root_nodes.hpp"
#include <mess/graph.hpp>

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace mess
{
namespace details
{
template <typename flat_graph, std::size_t... ordered, std::size_t first, std::size_t... others>
consteval auto recurse_on_siblings(indexes<ordered...>, indexes<first, others...>);

template <typename flat_graph, std::size_t current, std::size_t... ordered>
consteval auto recurse_on_successors_if_ready(indexes<ordered...>)
{
    using predecessors = unordered_predecessor_indexes<flat_graph, current>;
    constexpr bool is_ready = mess::contains<predecessors, indexes<ordered...>>;

    if constexpr (is_ready)
    {
        constexpr auto ordered_with_current = indexes<ordered..., current>();
        using successors =
            mess::to_indexes<flat_graph, typename std::tuple_element_t<current, flat_graph>::successor_tags>;

        if constexpr (successors::size == 0)
        {
            return ordered_with_current;
        }
        else
        {
            return recurse_on_siblings<flat_graph>(ordered_with_current, successors());
        }
    }
    else
    {
        return indexes<ordered...>();
    }
}

template <typename flat_graph, std::size_t... ordered, std::size_t first, std::size_t... others>
consteval auto recurse_on_siblings(indexes<ordered...>, indexes<first, others...>)
{
    constexpr auto ordered_successors = recurse_on_successors_if_ready<flat_graph, first>(indexes<ordered...>());

    if constexpr (sizeof...(others) == 0)
    {
        return ordered_successors;
    }
    else
    {
        return recurse_on_siblings<flat_graph>(ordered_successors, indexes<others...>());
    }
}

template <typename flat_graph> consteval auto ordered_indexes()
{
    return recurse_on_siblings<flat_graph>(indexes<>(), mess::root_indexes<flat_graph>());
}

template <typename flat_graph, std::size_t index, std::size_t... ordered>
consteval auto ordered_predecessors(indexes<ordered...>)
{
    using predecessors = unordered_predecessor_indexes<flat_graph, index>;
    return concatenate<
        std::conditional_t<contains(indexes<ordered>(), predecessors()), indexes<ordered>, indexes<>>...>();
}
} // namespace details

template <typename flat_graph, std::size_t index>
using ordered_predecessors =
    decltype(details::ordered_predecessors<flat_graph, index>(details::ordered_indexes<flat_graph>()));
} // namespace mess
