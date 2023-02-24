// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <set/contains.hpp>
#include <set/types.hpp>

#include <cassert>
#include <tuple>
#include <utility>

namespace dag
{
// Node of a directed acyclic graph
template <typename signature> struct node;
template <typename tag_, typename... predecessors_> struct node<tag_(predecessors_...)>
{
    static_assert(types::all_unique<tag_, predecessors_...>,
                  "tag_ must not appear in predecessors_..., and predecessors_'s must not repeat");

    using tag = tag_;
    using predecessors = types::set<predecessors_...>;
};

// Concept of a node of a directed graph.
// `T::predecessors` conforms to types::Set, and does not include `T::tag`.
template <typename T>
concept Node = requires(T) {
                   typename T::tag;
                   typename T::predecessors;
               } && types::Set<typename T::predecessors> && !
types::contains<typename T::predecessors, typename T::tag>;

namespace details
{
// Base template, fails to compile if the specialization is not triggered.
template <typename graph> constexpr struct fail all_nodes;
// Specialization to access the nodes.
template <template <typename...> typename set, typename... types>
constexpr bool all_nodes<set<types...>> = (Node<types> && ...);

// Base template, returns false.
template <typename graph> constexpr bool tags_unique = false;
// Specialization to access the nodes.
template <template <typename...> typename set, typename... nodes>
constexpr bool tags_unique<set<nodes...>> = types::all_unique<typename nodes::tag...>;
} // namespace details

// transform into an index_graph before looking for cycles
// So get that index_graph going, and then rewrite dag::everything for indexes
// Old question: why not allow dag::nodes to be created with numbers ? And convert that to std::integer_constant like
// before.

// Concept of a directed acyclic graph.
template <typename T>
concept Graph = types::Set<T> && details::all_nodes<T> && details::tags_unique<T>;

// Type of a directed acyclic graph.
template <Node... nodes_>
requires Graph<types::set<nodes_...>>
using graph = types::set<nodes_...>;
} // namespace dag