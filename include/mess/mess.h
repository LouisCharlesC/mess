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
#include <tuple>
#include <vector>

namespace mess
{
    template <std::size_t...>
    struct node_type;
    struct frame_type;

    // TODO: move elsewhere
    class multi_thread_kit
    {
        using value_type = int;

    public:
        template <std::size_t... Ts>
        explicit multi_thread_kit(const node_type<Ts...> &node);

        template <std::size_t notifying, std::size_t notified>
        static bool notify_and_check_if_ready(frame_type &frame);

        std::optional<value_type> result;

    private:
        // FIXME: must be atomic
        std::size_t _pending;
    };

    class sequential_kit
    {
        using value_type = int;

    public:
        template <std::size_t... Ts>
        explicit sequential_kit(const node_type<Ts...> &) {}

        template <std::size_t notifying, std::size_t notified>
        static bool notify_and_check_if_ready(frame_type &frame);

        std::optional<value_type> result;
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
        using kit = sequential_kit;
    };

    using executor_type = std_thread_executor;

    template <std::size_t... successors_index>
    struct node_type
    {
        static constexpr std::size_t successor_count = sizeof...(successors_index);

        std::function<int(int, int)> invocable;
        std::vector<std::size_t> arg_predecessors_index;
        std::vector<std::size_t> await_predecessors_index;
    };
    using flat_graph = std::tuple<node_type<1>, node_type<2, 3>, node_type<4>, node_type<4>, node_type<5>, node_type<6>, node_type<>>;

    struct frame_type
    {
        using executor = ::mess::executor_type;

        flat_graph graph;
        std::array<kit<executor>, std::tuple_size_v<flat_graph>> runtime;
        executor rt;
    };

    template <std::size_t index>
    void fetch_args_and_execute(frame_type &frame);

    template <std::size_t index>
    void thunk(int lhs, int rhs, frame_type &frame);

    frame_type make_frame(const flat_graph &graph, executor_type rt);

    void make_frame_and_run(const flat_graph &graph, executor_type rt);

    void run_and_take_care_of_deleting_the_frame(std::unique_ptr<frame_type> ptr);
} // namespace mess
