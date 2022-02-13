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

#include <mess/kits.hpp>
#include <mess/meta/list.hpp>

#include <memory>
#include <tuple>

namespace mess
{
    template <typename... nodes_type>
    using flat_graph = std::tuple<nodes_type...>;

    namespace details
    {
        template <typename executor_type, typename... nodes_type>
        static constexpr decltype(std::make_tuple(details::make_kit<executor_type, int, nodes_type>()...)) make_runtime_state(flat_graph<nodes_type...>) noexcept
        {
            return {};
        }

        template <typename executor_type, typename flat_graph>
        using runtime_state = decltype(make_runtime_state<executor_type>(std::declval<flat_graph>()));
    } // namespace details

    template <typename invocable_type, typename arg_predecessors_type, typename await_predecessors_type, typename successors_type>
    struct node_type
    {
        static_assert(is_arg_predecessors_type(arg_predecessors_type()), "mess::node_type's second template argument must be of type mess::arg_predecessors.");
        static_assert(is_await_predecessors_type(await_predecessors_type()), "mess::node_type's third template argument must be of type mess::await_predecessors.");
        static_assert(is_successors_type(successors_type()), "mess::node_type's forth template argument must be of type mess::successors.");

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
        // only create that if delete is needed :(
        [[no_unique_address]] typename kit_customizer<executor_type>::template latch_type<std::tuple_size_v<flat_graph> - 1> clean_up_latch;
    };
} // namespace mess