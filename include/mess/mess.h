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

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <thread>
#include <tuple>
#include <vector>

namespace mess
{
    template <typename invocable_type, typename arg_predecessors_type, typename successors_type>
    struct node_type;
    struct frame_type;

    // TODO: move elsewhere
    class multi_thread_kit
    {
        using value_type = int;

    public:
        template <typename invocable_type, typename arg_predecessors, typename successors>
        explicit multi_thread_kit(const node_type<invocable_type, arg_predecessors, successors> &node);

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
        template <typename invocable_type, typename arg_predecessors, typename successors>
        explicit sequential_kit(const node_type<invocable_type, arg_predecessors, successors> &node) {}

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

    class std_thread_executor
    {
    public:
        template <typename F>
        void execute(F &&f);

        void join();

    private:
        std::vector<std::thread> _threads;
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

    using executor_type = inline_executor;

    template <std::size_t... indexes>
    struct list
    {
        static constexpr std::size_t count = sizeof...(indexes);
        static constexpr bool empty = count == 0;
    };
    template <std::size_t... indexes>
    struct arg_predecessors : list<indexes...>
    {
    };
    template <std::size_t... indexes>
    struct await_predecessors : list<indexes...>
    {
    };
    template <std::size_t... indexes>
    struct successors : list<indexes...>
    {
    };

    template <std::size_t... indexes>
    constexpr bool is_arg_predecessors_type(arg_predecessors<indexes...>) { return true; }
    template <std::size_t... indexes>
    constexpr bool is_await_predecessors_type(await_predecessors<indexes...>) { return true; }
    template <std::size_t... indexes>
    constexpr bool is_successors_type(successors<indexes...>) { return true; }

    template <std::size_t... indexes>
    struct successors;

    template <typename invocable_type, typename arg_predecessors_type, typename successors_type>
    struct node_type
    {
        static_assert(is_arg_predecessors_type(arg_predecessors_type()), "mess::node_type's second template argument must be of type mess::arg_predecessors.");
        using arg_predecessors = arg_predecessors_type;
        static_assert(is_successors_type(successors_type()), "mess::node_type's forth template argument must be of type mess::successors_type.");
        using successors = successors_type;

        invocable_type invocable;
        std::vector<std::size_t> await_predecessors_index;
    };
    using flat_graph = std::tuple<node_type<int (&)(), arg_predecessors<>, successors<1>>,
                                  node_type<int (&)(int), arg_predecessors<0>, successors<2, 3>>,
                                  node_type<int (&)(int), arg_predecessors<1>, successors<4>>,
                                  node_type<int (&)(int), arg_predecessors<1>, successors<4>>,
                                  node_type<int (&)(int, int), arg_predecessors<2, 3>, successors<5>>,
                                  node_type<int (&)(int), arg_predecessors<4>, successors<>>>;

    struct frame_type
    {
        using executor = ::mess::executor_type;

        flat_graph graph;
        std::array<kit<executor>, std::tuple_size_v<flat_graph>> runtime;
        executor rt;
    };

    template <std::size_t index>
    void fetch_args_and_execute(frame_type &frame);

    template <std::size_t index, typename... args_type>
    void thunk(frame_type &frame, args_type... args);

    frame_type make_frame(const flat_graph &graph, executor_type rt);

    void make_frame_and_run(const flat_graph &graph, executor_type rt);

    void run_and_take_care_of_deleting_the_frame(std::unique_ptr<frame_type> ptr);
} // namespace mess
