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

#include <utility>

namespace mess
{
    template <typename F>
    void std_thread_executor::execute(F &&f)
    {
        _threads.emplace_back(std::forward<F>(f));
    }
    void std_thread_executor::join()
    {
        for (auto &thread : _threads)
        {
            thread.join();
        }
    }

    template <typename invocable_type, typename arg_predecessors, typename successors>
    multi_thread_kit::multi_thread_kit(const node_type<invocable_type, arg_predecessors, successors> &node) : _pending(arg_predecessors::count + node.await_predecessors_index.size())
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

    template <std::size_t v>
    struct get
    {
        static constexpr std::size_t value = v;
    };
    template <std::size_t index, std::size_t... indexes>
    static constexpr bool is_last(arg_predecessors<indexes...>)
    {
        // the <()> syntax uses the comma operator of std::size_t to only use the last item, effectively resulting in get<last_index>
        return index == get<(indexes, ...)>::value;
    }
    template <std::size_t>
    static constexpr bool is_last(arg_predecessors<>)
    {
        return false;
    }

    template <std::size_t notifying, std::size_t notified>
    bool sequential_kit::notify_and_check_if_ready(frame_type &frame)
    {
        return std::get<notified>(frame.graph).await_predecessors_index.empty() && is_last<notifying>(typename std::tuple_element_t<notified, flat_graph>::arg_predecessors()) ||
               !std::get<notified>(frame.graph).await_predecessors_index.empty() && std::get<notified>(frame.graph).await_predecessors_index.back() == notifying;
    }

    template <std::size_t index, std::size_t... arg_predecessors_index>
    void fetch_args_and_execute(frame_type &frame, arg_predecessors<arg_predecessors_index...>)
    {
        frame.rt.execute([&runtime = frame.runtime, &frame]()
                         { thunk<index>(frame, *runtime[arg_predecessors_index].result...); });
    }

    template <std::size_t index>
    void fetch_args_and_execute(frame_type &frame)
    {
        fetch_args_and_execute<index>(frame, typename std::tuple_element_t<index, flat_graph>::arg_predecessors());
    }

    template <std::size_t notifying, std::size_t notified>
    static void notify_and_execute_if_ready(frame_type &frame)
    {
        if (kit<executor_type>::notify_and_check_if_ready<notifying, notified>(frame))
        {
            fetch_args_and_execute<notified>(frame);
        }
    }

    template <std::size_t notifying, typename invocable_type, typename arg_predecessors, std::size_t... successors_index>
    static void notify_and_execute_ready_successors(frame_type &frame, node_type<invocable_type, arg_predecessors, successors<successors_index...>> &)
    {
        (notify_and_execute_if_ready<notifying, successors_index>(frame), ...);
    }

    template <std::size_t notifying>
    static void notify_and_execute_ready_successors(frame_type &frame)
    {
        notify_and_execute_ready_successors<notifying>(frame, std::get<notifying>(frame.graph));
    }

    template <std::size_t index, typename... args_type>
    void thunk(frame_type &frame, args_type... args)
    {
        // Do not store the result if there are no successors interested in it.
        if constexpr (!std::tuple_element_t<index, flat_graph>::successors::empty)
        {
            frame.runtime[index].result = std::get<index>(frame.graph).invocable(args...);

            notify_and_execute_ready_successors<index>(frame);
        }
        else
        {
            std::get<index>(frame.graph).invocable(args...);
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
        if (std::tuple_element_t<index, flat_graph>::arg_predecessors::empty && std::get<index>(frame.graph).await_predecessors_index.empty())
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

        // std::get<std::tuple_size_v<flat_graph> - 1>(frame.graph) = mess::node_type<>{[ptr = std::shared_ptr(std::move(ptr))](int, int) mutable -> int
        //                                                                              {
        //                                                                                  std::shared_ptr release(std::move(ptr));
        //                                                                                  return 0;
        //                                                                              },
        //                                                                              {},
        //                                                                              {5}};
        // std::get<std::tuple_size_v<flat_graph> - 1>(frame.runtime) = kit<executor_type>(std::get<std::tuple_size_v<flat_graph> - 1>(frame.graph));

        execute_root_nodes(frame);
    }
} // namespace mess
