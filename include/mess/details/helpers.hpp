/**
 * @file helpers.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <mess/frame.hpp>
#include <mess/meta/leaf_nodes.hpp>
#include <mess/meta/root_nodes.hpp>

#include <cstdint>
#include <memory>
#include <tuple>
#include <utility>

namespace mess
{
    namespace details
    {
        template <bool is_self_delete>
        struct invoker
        {
            template <std::size_t index, typename executor_type, typename flat_graph, typename... args_type>
            static void thunk(frame_type<executor_type, flat_graph> &frame, const args_type &...args) noexcept
            {
                if constexpr (std::is_same_v<details::invoke_result<flat_graph, index>, void>)
                {
                    // Do not try to store the result if it is void.
                    std::get<index>(frame.graph).invocable(args...);
                }
                else
                {
                    std::get<index>(frame.runtime).result = std::get<index>(frame.graph).invocable(args...);
                }

                if constexpr (!is_leaf<flat_graph, index>)
                {
                    // Non-leaf nodes notify their successors.
                    notify_and_execute_ready_successors<index>(frame);
                }
                else if (is_self_delete)
                {
                    // Leaf nodes notify the self_deleter if needed.
                    if (frame.self_deleter.template notify_and_check_if_ready<index>())
                    {
                        delete (std::addressof(frame));
                    }
                }
            }

            template <std::size_t index, typename executor_type, typename flat_graph>
            static void fetch_args_and_execute(frame_type<executor_type, flat_graph> &frame)
            {
                [&frame]<std::size_t... arg_predecessors_index>(std::index_sequence<arg_predecessors_index...>)
                {
                    // TODO: should executor return bool to indicate it was stopped ? or be polled with is_stop_requested() or something
                    frame.executor.execute([&frame]()
                                           { thunk<index>(frame, *std::get<arg_predecessors_index>(frame.runtime).result...); });
                }
                (typename std::tuple_element_t<index, flat_graph>::arg_predecessors());
            }

            template <std::size_t notifying, std::size_t notified, typename executor_type, typename flat_graph>
            static void notify_and_execute_if_ready(frame_type<executor_type, flat_graph> &frame)
            {
                if (std::get<notified>(frame.runtime).latch.template notify_and_check_if_ready<notifying>())
                {
                    fetch_args_and_execute<notified>(frame);
                }
            }

            template <std::size_t notifier, typename executor_type, typename flat_graph>
            static void notify_and_execute_ready_successors(frame_type<executor_type, flat_graph> &frame)
            {
                [&frame]<std::size_t... successors_index>(std::index_sequence<successors_index...>)
                {
                    (notify_and_execute_if_ready<notifier, successors_index>(frame), ...);
                }
                (typename std::tuple_element_t<notifier, flat_graph>::successors());
            }

            template <typename executor_type, typename flat_graph>
            static void execute_root_nodes(frame_type<executor_type, flat_graph> &frame)
            {
                [&frame]<std::size_t... indexes>(std::index_sequence<indexes...>)
                {
                    [&frame]<std::size_t... root_nodes_index>(std::index_sequence<root_nodes_index...>)
                    {
                        (fetch_args_and_execute<root_nodes_index>(frame), ...);
                    }
                    (root_nodes<flat_graph, indexes...>());
                }
                (std::make_index_sequence<std::tuple_size_v<flat_graph>>());
            }
        };
    } // namespace details
} // namespace mess