// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <mess/graph.hpp>
#include <mess/meta/leaf_nodes.hpp>
#include <mess/meta/root_nodes.hpp>
#include <mess/meta/sequences.hpp>

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
    template <std::size_t index, typename scheduler_type, typename flat_graph, typename... arg_types>
    static void invoke(frame_type<scheduler_type, flat_graph> &frame, const arg_types &...args) noexcept
    {
        if constexpr (std::is_same_v<details::invoke_result_t<flat_graph, index>, void> ||
                      successor_indexes<flat_graph, index>::size == 0)
        {
            // Do not try to store the result if it is void or if there are no successors.
            std::get<index>(frame._graph).invocable(args...);
        }
        else
        {
            std::get<index>(frame._runtime).result = std::get<index>(frame._graph).invocable(args...);
        }
    }

    template <std::size_t index, typename scheduler_type, typename flat_graph, typename... arg_types>
    static void invoke_taking_care_of_exceptions_and_cancelation(frame_type<scheduler_type, flat_graph> &frame,
                                                                 const arg_types &...args) noexcept
    {
        if constexpr (noexcept(std::get<index>(frame._graph).invocable(args...)))
        {
            invoke<index>(frame, args...);
        }
        else
        {
            try
            {
                invoke<index>(frame, args...);
            }
            catch (...)
            {
                // cancel_leaf_nodes();
            }
        }
    }

    template <std::size_t index, typename scheduler_type, typename flat_graph>
    static void notify_after_invocation(frame_type<scheduler_type, flat_graph> &frame)
    {
        if constexpr (!is_leaf<flat_graph, index>)
        {
            // Non-leaf nodes notify their successors.
            notify_and_schedule_ready_successors<index>(frame);
        }
        else if (is_self_delete)
        {
            // Leaf nodes notify the self_deleter if needed.
            if (frame._done_latch.template notify_and_check_if_ready<index>())
            {
                // And when everything is done executing, the frame gets deleted.
                delete (std::addressof(frame));
            }
        }
    }

    template <std::size_t index, typename scheduler_type, typename flat_graph, typename... arg_types>
    static void thunk(frame_type<scheduler_type, flat_graph> &frame, const arg_types &...args) noexcept
    {
        invoke_taking_care_of_exceptions_and_cancelation<index>(frame, args...);

        notify_after_invocation<index>(frame);
    }

    template <std::size_t index, typename scheduler_type, typename flat_graph>
    static void fetch_args_and_schedule(frame_type<scheduler_type, flat_graph> &frame) noexcept
    {
        [&frame]<std::size_t... arg_predecessors>(indexes<arg_predecessors...>)
        {
            assert((std::get<arg_predecessors>(frame._runtime).result && ...));
            frame._scheduler.schedule(
                [&frame]() { thunk<index>(frame, *std::get<arg_predecessors>(frame._runtime).result...); });
        }
        (arg_predecessor_indexes<flat_graph, index>());
    }

    template <std::size_t notifying, std::size_t notified, typename scheduler_type, typename flat_graph>
    static void notify_and_schedule_if_ready(frame_type<scheduler_type, flat_graph> &frame) noexcept
    {
        if (std::get<notified>(frame._runtime).latch.template notify_and_check_if_ready<notifying>())
        {
            // if constexpr (!all_noexcept)
            {
                //     if (is_cancelled)
                //     {
                //         notify_after_invocation<notify_after_invocation>(frame);

                //         return;
                //     }
                // }

                fetch_args_and_schedule<notified>(frame);
            }
        }
    }

    template <std::size_t notifier, typename scheduler_type, typename flat_graph>
    static void notify_and_schedule_ready_successors(frame_type<scheduler_type, flat_graph> &frame) noexcept
    {
        [&frame]<std::size_t... successors_index>(indexes<successors_index...>)
        {
            (notify_and_schedule_if_ready<notifier, successors_index>(frame), ...);
        }
        (successor_indexes<flat_graph, notifier>());
    }

    template <typename scheduler_type, typename flat_graph>
    static void schedule_root_nodes(frame_type<scheduler_type, flat_graph> &frame) noexcept
    {
        [&frame]<std::size_t... root_node_indexes>(indexes<root_node_indexes...>)
        {
            (frame._scheduler.schedule([&frame]() { thunk<root_node_indexes>(frame); }), ...);
        }
        (mess::root_indexes<flat_graph>());
    }
};
} // namespace details
} // namespace mess