/**
 * @file kits.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <mess/executors/inline.hpp>
#include <mess/executors/std_thread.hpp>
#include <mess/latches/atomic_countdown.hpp>
#include <mess/latches/last_predeccors.hpp>

#include <cstdint>
#include <optional>

namespace mess
{
    template <typename value_type>
    struct optional_result
    {
        std::optional<value_type> result;
    };

    template <>
    struct optional_result<void>
    {
    };

    template <typename executor_type>
    struct kit_customizer
    {
        template <std::size_t... predecessors>
        using latch_type = atomic_countdown_latch<predecessors...>;
        template <typename value_type>
        using result_type = optional_result<value_type>;
    };

    template <>
    struct kit_customizer<inline_executor>
    {
        template <std::size_t... predecessors>
        using latch_type = last_predecessor_latch<predecessors...>;
        template <typename value_type>
        using result_type = optional_result<value_type>;
    };

    template <typename executor_type, typename value_type, std::size_t... predecessors>
    struct kit : kit_customizer<executor_type>::template latch_type<predecessors...>,
                 kit_customizer<executor_type>::template result_type<value_type>
    {
    };

    namespace details
    {
        template <typename executor_type, typename value_type, std::size_t... predecessors>
        static constexpr auto make_kit(details::list<predecessors...>)
        {
            return kit<executor_type, value_type, predecessors...>();
        }
        template <typename executor_type, typename value_type, typename node_type>
        static constexpr auto make_kit()
        {
            using arg_predecessors_list = typename node_type::arg_predecessors::list;
            using await_predecessors_list = typename node_type::await_predecessors::list;
            using predecessors = mess::concatenate<arg_predecessors_list, await_predecessors_list>;

            return make_kit<executor_type, value_type>(predecessors());
        }
    } // namespace details
} // namespace mess