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

    bool multi_thread_kit::notify_and_check_if_ready(frame &frame, std::size_t notified, std::size_t)
    {
        return --frame.graph[notified].pending_count == 0;
    }

    template <typename F>
    void inline_executor::execute(F &&f)
    {
        std::forward<F>(f)();
    }

    bool sequential_kit::notify_and_check_if_ready(frame &frame, std::size_t notified, std::size_t notifying)
    {
        // FIXME: Not gonna work with non-arg predeccessors.
        return frame.graph[notified].predecessors_index.empty() || frame.graph[notified].predecessors_index.back() == notifying;
    }

    void fetch_args_and_execute(frame &frame, std::size_t index)
    {
        int lhs = 1;
        int rhs = 1;
        if (frame.graph[index].predecessors_index.size() > 0)
        {
            lhs = *frame.results[frame.graph[index].predecessors_index[0]];

            if (frame.graph[index].predecessors_index.size() > 1)
            {
                rhs = *frame.results[frame.graph[index].predecessors_index[1]];
            }
        }

        frame.rt.execute([lhs, rhs, &frame, index]()
                         { thunk(lhs, rhs, frame, index); });
    }

    void thunk(int lhs, int rhs, frame &frame, std::size_t index)
    {
        // This if only to avoid accessing non existing result for the task that deletes the frame.
        if (!frame.graph[index].successors_index.empty())
        {
            frame.results[index] = frame.graph[index].invocable(lhs, rhs);

            const auto successors = frame.graph[index].successors_index;
            for (const int successor : successors)
            {
                if (kit<executor>::notify_and_check_if_ready(frame, successor, index))
                {
                    fetch_args_and_execute(frame, successor);
                }
            }
        }
        else
        {
            frame.graph[index].invocable(lhs, rhs);
        }
    }

    frame make_frame(const flat_graph &graph, executor rt)
    {
        return {graph, {}, rt};
    }

    static void run(frame &frame)
    {
        const auto graph = frame.graph;
        for (std::size_t i = 0; i < graph.size(); ++i)
        {
            if (graph[i].pending_count == 0)
            {
                fetch_args_and_execute(frame, i);
            }
        }
    }

    void make_frame_and_run(const flat_graph &graph, executor rt)
    {
        auto frame = make_frame(graph, rt);

        run(frame);
    }

    // bool add_guard_successor(std::vector<std::size_t> &successors)
    // {
    // }

    void run_and_take_care_of_deleting_the_frame(std::unique_ptr<frame> ptr)
    {
        auto &frame = *ptr;

        std::size_t numberOfPredessors = 0;
        for (auto node = frame.graph.begin(); node != frame.graph.end() - 1; ++node)
        {
            if (node->successors_index.empty())
            {
                node->successors_index.push_back(number_of_nodes);
                ++numberOfPredessors;
            }
        }
        std::get<number_of_nodes>(frame.graph) = mess::node{[ptr = std::shared_ptr(std::move(ptr))](int, int) mutable -> int
                                                            {
                                                                std::shared_ptr release(std::move(ptr));
                                                                return 0;
                                                            },
                                                            {},
                                                            numberOfPredessors,
                                                            {}};

        run(frame);
    }
} // namespace mess
