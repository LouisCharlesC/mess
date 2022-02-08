/**
 * @file mess.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-12-05
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "mess/mess.h"

#include <thread>
#include <utility>

namespace mess
{
    template <typename F>
    void std_thread_executor::execute(F &&f)
    {
        std::thread(std::forward<F>(f))
            .detach();
    }

    template <std::size_t... Ts>
    multi_thread_kit::multi_thread_kit(const node_type<Ts...> &node) : _pending(node.arg_predecessors_index.size() + node.await_predecessors_index.size())
    {
    }

    template <std::size_t, std::size_t notified>
    bool multi_thread_kit::notify_and_check_if_ready(frame_type &frame)
    {
        return --std::get<notified>(frame.runtime)._pending == 0;
    }

    template <typename F>
    void inline_executor::execute(F &&f)
    {
        std::forward<F>(f)();
    }

    template <std::size_t notifying, std::size_t notified>
    bool sequential_kit::notify_and_check_if_ready(frame_type &frame)
    {
        return std::get<notified>(frame.graph).await_predecessors_index.empty() && std::get<notified>(frame.graph).arg_predecessors_index.back() == notifying ||
               !std::get<notified>(frame.graph).await_predecessors_index.empty() && std::get<notified>(frame.graph).await_predecessors_index.back() == notifying;
    }

    template <std::size_t index>
    void fetch_args_and_execute(frame_type &frame)
    {
        int lhs = 1;
        int rhs = 1;
        if (std::get<index>(frame.graph).arg_predecessors_index.size() > 0)
        {
            lhs = *frame.runtime[std::get<index>(frame.graph).arg_predecessors_index[0]].result;

            if (std::get<index>(frame.graph).arg_predecessors_index.size() > 1)
            {
                rhs = *frame.runtime[std::get<index>(frame.graph).arg_predecessors_index[1]].result;
            }
        }

        frame.rt.execute([lhs, rhs, &frame]()
                         { thunk<index>(lhs, rhs, frame); });
    }

    template <std::size_t notifying, std::size_t notified>
    static void notify_and_execute_if_ready(frame_type &frame)
    {
        if (kit<executor_type>::notify_and_check_if_ready<notifying, notified>(frame))
        {
            fetch_args_and_execute<notified>(frame);
        }
    }

    template <std::size_t notifying, std::size_t... successors>
    static void notify_and_execute_ready_successors(frame_type &frame, node_type<successors...> &)
    {
        (notify_and_execute_if_ready<notifying, successors>(frame), ...);
    }

    template <std::size_t notifying>
    static void notify_and_execute_ready_successors(frame_type &frame)
    {
        notify_and_execute_ready_successors<notifying>(frame, std::get<notifying>(frame.graph));
    }

    template <std::size_t index>
    void thunk(int lhs, int rhs, frame_type &frame)
    {
        // Do not store the result if there are no successors interested in it.
        if constexpr (std::tuple_element_t<index, flat_graph>::successor_count != 0)
        {
            frame.runtime[index].result = std::get<index>(frame.graph).invocable(lhs, rhs);

            notify_and_execute_ready_successors<index>(frame);
        }
        else
        {
            std::get<index>(frame.graph).invocable(lhs, rhs);
        }
    }

    template <std::size_t... indexes>
    static frame_type make_frame(const flat_graph &graph, executor_type executor, std::index_sequence<indexes...>)
    {
        return {graph, {kit<executor_type>(std::get<indexes>(graph))...}, executor};
    }

    frame_type make_frame(const flat_graph &graph, executor_type executor)
    {
        return make_frame(graph, executor, std::make_index_sequence<std::tuple_size_v<flat_graph>>());
    }

    template <std::size_t index>
    static void execute_if_root(frame_type &frame)
    {
        if (std::get<index>(frame.graph).arg_predecessors_index.empty() && std::get<index>(frame.graph).await_predecessors_index.empty())
        {
            fetch_args_and_execute<index>(frame);
        }
    }

    template <std::size_t... indexes>
    static void execute_root_nodes(frame_type &frame, std::index_sequence<indexes...>)
    {
        (execute_if_root<indexes>(frame), ...);
    }

    static void execute_root_nodes(frame_type &frame)
    {
        execute_root_nodes(frame, std::make_index_sequence<std::tuple_size_v<flat_graph>>());
    }

    void make_frame_and_run(const flat_graph &graph, executor_type rt)
    {
        auto frame = make_frame(graph, rt);

        execute_root_nodes(frame);
    }

    void run_and_take_care_of_deleting_the_frame(std::unique_ptr<frame_type> ptr)
    {
        auto &frame = *ptr;

        std::get<std::tuple_size_v<flat_graph> - 1>(frame.graph) = mess::node_type<>{[ptr = std::shared_ptr(std::move(ptr))](int, int) mutable -> int
                                                                                     {
                                                                                         std::shared_ptr release(std::move(ptr));
                                                                                         return 0;
                                                                                     },
                                                                                     {},
                                                                                     {5}};
        std::get<std::tuple_size_v<flat_graph> - 1>(frame.runtime) = kit<executor_type>(std::get<std::tuple_size_v<flat_graph> - 1>(frame.graph));

        execute_root_nodes(frame);
    }
} // namespace mess
