// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <mess/kit.hpp>
#include <mess/meta/concatenate.hpp>
#include <mess/meta/leaf_nodes.hpp>
#include <mess/meta/sequences.hpp>

#include <cassert>
#include <tuple>
#include <utility>

namespace mess
{
template <typename... nodes_type> using flat_graph = std::tuple<nodes_type...>;

template <typename tag_t, typename invocable_type_t, typename arg_predecessors_t, typename successors_t,
          typename other_predecessors_t>
struct node_type
{
    static_assert(is_arg_predecessors_type(arg_predecessors_t()),
                  "mess::node_type's second template argument must be of type mess::args.");
    static_assert(is_other_predecessors_type(other_predecessors_t()),
                  "mess::node_type's third template argument must be of type mess::after.");
    static_assert(is_successors_type(successors_t()),
                  "mess::node_type's fourth template argument must be of type mess::successors.");

    using tag = tag_t;
    using invocable_type = invocable_type_t;
    using arg_predecessors = arg_predecessors_t;
    using other_predecessors = other_predecessors_t;
    using unordered_predecessors = concatenate<arg_predecessors, other_predecessors>;
    using successors = successors_t;

    invocable_type invocable;
};

template <typename scheduler_type, typename flat_graph> class frame_type
{
  public:
    frame_type(scheduler_type &scheduler, flat_graph graph) : _scheduler(scheduler), _graph(std::move(graph))
    {
    }

    // private:
    [[no_unique_address]] flat_graph _graph;
    [[no_unique_address]] runtime_state<scheduler_type, flat_graph> _runtime;
    [[no_unique_address]] leaf_nodes_latch<scheduler_type, flat_graph> _leafs_latch;
    scheduler_type &_scheduler;
};
} // namespace mess