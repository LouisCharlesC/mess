/**
 * @file frame.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <mess/kit.hpp>
#include <mess/meta/leaf_nodes.hpp>
#include <mess/meta/sequences.hpp>

#include <cassert>
#include <tuple>
#include <utility>

namespace mess
{
  template <typename... nodes_type>
  using flat_graph = std::tuple<nodes_type...>;

  template <typename invocable_type_arg,
            typename arg_predecessors_type,
            typename other_predecessors_type,
            typename successors_type>
  struct node_type
  {
    static_assert(is_arg_predecessors_type(arg_predecessors_type()),
                  "mess::node_type's second template argument must be of type "
                  "mess::arg_predecessors.");
    static_assert(is_other_predecessors_type(other_predecessors_type()),
                  "mess::node_type's third template argument must be of type "
                  "mess::other_predecessors.");
    static_assert(is_successors_type(successors_type()),
                  "mess::node_type's forth template argument must be of type "
                  "mess::successors.");

    using invocable_type = invocable_type_arg;
    using arg_predecessors = arg_predecessors_type;
    using other_predecessors = other_predecessors_type;
    using unordered_predecessors =
        concatenate<arg_predecessors_type, other_predecessors_type>;
    using successors = successors_type;

    invocable_type invocable;
  };

  template <typename scheduler_type, typename flat_graph>
  class frame_type
  {
  public:
    frame_type(scheduler_type &scheduler, flat_graph graph)
        : _scheduler(scheduler), _graph(std::move(graph)) {}

    // private:
    scheduler_type &_scheduler;
    [[no_unique_address]] flat_graph _graph;
    [[no_unique_address]] runtime_state<scheduler_type, flat_graph> _runtime;
    [[no_unique_address]] leaf_nodes_latch<scheduler_type, flat_graph> _leafs_latch;
  };
} // namespace mess