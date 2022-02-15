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

#include <cstdint>
#include <memory>
#include <tuple>
#include <utility>

namespace mess
{
    namespace details
    {
        template <std::size_t index, typename flat_graph>
        using arg_predecessors = typename std::tuple_element_t<index, flat_graph>::arg_predecessors;
        template <std::size_t index, typename flat_graph>
        using await_predecessors = typename std::tuple_element_t<index, flat_graph>::await_predecessors;
        template <std::size_t index, typename flat_graph>
        using successors = typename std::tuple_element_t<index, flat_graph>::successors;

        template <bool is_self_delete>
        struct self_delete
        {
            template <std::size_t index, typename executor_type, typename flat_graph, typename... args_type>
            static void thunk(frame_type<executor_type, flat_graph> &frame, const args_type &...args) noexcept
            {
                // Do not store the result if there are no successors interested in it.
                if constexpr (!successors<index, flat_graph>::empty)
                {
                    // TODO: use std::invoke
                    std::get<index>(frame.runtime).result = std::get<index>(frame.graph).invocable(args...);

                    notify_and_execute_ready_successors<index>(frame);
                }
                else
                {
                    std::get<index>(frame.graph).invocable(args...);

                    if constexpr (is_self_delete)
                    {
                        if (frame.self_delete_latch.template notify_and_check_if_ready<index>())
                        {
                            delete (std::addressof(frame));
                        }
                    }
                }
            }

            template <std::size_t index, typename executor_type, typename flat_graph, std::size_t... arg_predecessors_index>
            static void fetch_args_and_execute(frame_type<executor_type, flat_graph> &frame, mess::arg_predecessors<arg_predecessors_index...>)
            {
                // TODO: should executor return bool to indicate it was stopped ? or be polled with is_stop_requested() or something
                frame.executor.execute([&frame]()
                                       { thunk<index>(frame, *std::get<arg_predecessors_index>(frame.runtime).result...); });
            }

            template <std::size_t index, typename executor_type, typename flat_graph>
            static void fetch_args_and_execute(frame_type<executor_type, flat_graph> &frame)
            {
                fetch_args_and_execute<index>(frame, arg_predecessors<index, flat_graph>());
            }

            template <std::size_t notifying, std::size_t notified, typename executor_type, typename flat_graph>
            static void notify_and_execute_if_ready(frame_type<executor_type, flat_graph> &frame)
            {
                if (std::get<notified>(frame.runtime).latch.template notify_and_check_if_ready<notifying>())
                {
                    fetch_args_and_execute<notified>(frame);
                }
            }

            template <std::size_t notifying, typename executor_type, typename flat_graph, std::size_t... successors_index>
            static void notify_and_execute_ready_successors(frame_type<executor_type, flat_graph> &frame, mess::successors<successors_index...>)
            {
                (notify_and_execute_if_ready<notifying, successors_index>(frame), ...);
            }

            template <std::size_t notifying, typename executor_type, typename flat_graph>
            static void notify_and_execute_ready_successors(frame_type<executor_type, flat_graph> &frame)
            {
                notify_and_execute_ready_successors<notifying>(frame, successors<notifying, flat_graph>());
            }

            template <std::size_t index, typename executor_type, typename flat_graph>
            static void execute_if_root(frame_type<executor_type, flat_graph> &frame)
            {
                if constexpr (arg_predecessors<index, flat_graph>::empty &&
                              await_predecessors<index, flat_graph>::empty)
                {
                    fetch_args_and_execute<index>(frame);
                }
            }

            template <typename executor_type, typename flat_graph, std::size_t... indexes>
            static void execute_root_nodes(frame_type<executor_type, flat_graph> &frame, std::index_sequence<indexes...>)
            {
                (execute_if_root<indexes>(frame), ...);
            }

            template <typename executor_type, typename flat_graph>
            static void execute_root_nodes(frame_type<executor_type, flat_graph> &frame)
            {
                execute_root_nodes(frame, std::make_index_sequence<std::tuple_size_v<flat_graph>>());
            }
        };
    } // namespace details
} // namespace mess