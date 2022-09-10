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
template <bool is_self_delete> struct invoker
{
    template <std::size_t index, typename scheduler_type, typename flat_graph, typename... args_type>
    static void thunk(frame_type<scheduler_type, flat_graph> &frame, const args_type &...args) noexcept
    {
        if constexpr (std::is_same_v<details::invoke_result_t<flat_graph, index>, void>)
        {
            // Do not try to store the result if it is void.
            std::get<index>(frame._graph).invocable(args...);
        }
        else
        {
            std::get<index>(frame._runtime).result = std::get<index>(frame._graph).invocable(args...);
        }

        if constexpr (!is_leaf<flat_graph, index>)
        {
            // Non-leaf nodes notify their successors.
            notify_and_schedule_ready_successors<index>(frame);
        }
        else if (is_self_delete)
        {
            // Leaf nodes notify the self_deleter if needed.
            if (frame._leafs_latch.template notify_and_check_if_ready<index>())
            {
                delete (std::addressof(frame));
            }
        }
    }

    template <std::size_t index_v, typename scheduler_type, typename flat_graph>
    static void fetch_args_and_schedule(frame_type<scheduler_type, flat_graph> &frame)
    {
        [&frame]<typename... arg_predecessors_tags>(tags<arg_predecessors_tags...>) {
            // TODO: should scheduler return bool to indicate it was stopped ? or be polled with is_stop_requested() or
            // something
            frame._scheduler.schedule([&frame]() {
                thunk<index_v>(frame, *std::get<index<arg_predecessors_tags, flat_graph>>(frame._runtime).result...);
            });
        }(typename std::tuple_element_t<index_v, flat_graph>::arg_predecessors());
    }

    template <std::size_t notifying, std::size_t notified, typename scheduler_type, typename flat_graph>
    static void notify_and_schedule_if_ready(frame_type<scheduler_type, flat_graph> &frame)
    {
        if (std::get<notified>(frame._runtime).latch.template notify_and_check_if_ready<notifying>())
        {
            fetch_args_and_schedule<notified>(frame);
        }
    }

    template <std::size_t notifier, typename scheduler_type, typename flat_graph>
    static void notify_and_schedule_ready_successors(frame_type<scheduler_type, flat_graph> &frame)
    {
        [&frame]<std::size_t... successors_index>(std::index_sequence<successors_index...>)
        {
            (notify_and_schedule_if_ready<notifier, successors_index>(frame), ...);
        }
        (typename std::tuple_element_t<notifier, flat_graph>::successors());
    }

    template <typename scheduler_type, typename flat_graph>
    static void scheduler_root_nodes(frame_type<scheduler_type, flat_graph> &frame)
    {
        [&frame]<std::size_t... indexes_v>(std::index_sequence<indexes_v...>)
        {
            [&frame]<std::size_t... root_nodes_index>(indexes<root_nodes_index...>)
            {
                (fetch_args_and_schedule<root_nodes_index>(frame), ...);
            }
            (root_nodes<flat_graph, indexes_v...>());
        }
        (std::make_index_sequence<std::tuple_size_v<flat_graph>>());
    }
};
} // namespace details
} // namespace mess