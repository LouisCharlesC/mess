/**
 * @file mess.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-12-02
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

namespace mess
{
    constexpr std::size_t number_of_nodes = 6;

    struct frame;

    // TODO: move elsewhere
    struct multi_thread_kit
    {
        // FIXME: must be atomic
        using count_down_latch = std::size_t;

        static bool notify_and_check_if_ready(frame &frame, std::size_t notified, std::size_t notifying);
    };

    struct sequential_kit
    {
        using count_down_latch = std::size_t;

        static bool notify_and_check_if_ready(frame &frame, std::size_t notified, std::size_t notifying);
    };
    // end move elsewhere

    template <typename Executor>
    struct kit_customizer
    {
        using kit = multi_thread_kit;
    };
    template <typename Executor>
    using kit = typename kit_customizer<Executor>::kit;

    struct std_thread_executor
    {
        template <typename F>
        void execute(F &&f);
    };

    struct inline_executor
    {
        template <typename F>
        void execute(F &&f);
    };
    template <>
    struct kit_customizer<inline_executor>
    {
        using kit = multi_thread_kit;
    };

    using executor = inline_executor;

    struct node
    {
        using executor = ::mess::executor;

        std::function<int(int, int)> invocable;
        std::vector<std::size_t> predecessors_index;
        kit<executor>::count_down_latch pending_count;
        std::vector<std::size_t> successors_index;
    };
    using flat_graph = std::array<mess::node, number_of_nodes + 1>;

    struct frame
    {
        using executor = ::mess::executor;

        flat_graph graph;
        std::array<std::optional<int>, number_of_nodes> results;
        executor rt;
    };

    void fetch_args_and_execute(frame &frame, std::size_t index);

    void thunk(int lhs, int rhs, frame &frame, std::size_t index);

    frame make_frame(const flat_graph &graph, executor rt);

    void make_frame_and_run(const flat_graph &graph, executor rt);

    void run_and_take_care_of_deleting_the_frame(std::unique_ptr<frame> ptr);
} // namespace mess
