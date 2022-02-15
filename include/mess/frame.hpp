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
#include <mess/meta/list.hpp>

#include <tuple>
#include <utility>

namespace mess
{
    template <typename... nodes_type>
    using flat_graph = std::tuple<nodes_type...>;

    template <typename invocable_type_arg, typename arg_predecessors_type, typename await_predecessors_type, typename successors_type>
    struct node_type
    {
        static_assert(is_arg_predecessors_type(arg_predecessors_type()), "mess::node_type's second template argument must be of type mess::arg_predecessors.");
        static_assert(is_await_predecessors_type(await_predecessors_type()), "mess::node_type's third template argument must be of type mess::await_predecessors.");
        static_assert(is_successors_type(successors_type()), "mess::node_type's forth template argument must be of type mess::successors.");

        using invocable_type = invocable_type_arg;
        using arg_predecessors = arg_predecessors_type;
        using await_predecessors = await_predecessors_type;
        using successors = successors_type;

        invocable_type invocable;
    };

    template <typename executor_type, typename flat_graph>
    struct frame_type
    {
        template <typename flat_graph_arg>
        frame_type(executor_type &executor, flat_graph_arg &&graph) : executor(executor),
                                                                      graph(std::forward<flat_graph_arg>(graph))
        {
        }

        // TODO: static check node types and flat_graph
        using runtime_state = details::runtime_state<executor_type, flat_graph>;

        executor_type &executor;
        flat_graph graph;
        runtime_state runtime;
        [[no_unique_address]] typename kit_customizer<executor_type>::template latch_type<5, 7> self_delete_latch;
    };
} // namespace mess