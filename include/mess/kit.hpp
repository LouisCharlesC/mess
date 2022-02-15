/**
 * @file kit.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <mess/kit_customizer.hpp>
#include <mess/latches/ready.hpp>
#include <mess/meta/concatenate.hpp>
#include <mess/meta/graph.hpp>
#include <mess/meta/list.hpp>

#include <cstdint>
#include <utility>

namespace mess
{
    template <typename executor_type, typename value_type, std::size_t... predecessors>
    struct kit
    {
        typename kit_customizer<executor_type>::template result_type<value_type> result;
        typename kit_customizer<executor_type>::template latch_type<predecessors...> latch;
    };

    // void: no need for a result.
    template <typename executor_type, std::size_t... predecessors>
    struct kit<executor_type, void, predecessors...>
    {
        typename kit_customizer<executor_type>::template latch_type<predecessors...> latch;
    };

    // One predecessor: use a ready latch.
    template <typename executor_type, typename value_type, std::size_t predecessor>
    struct kit<executor_type, value_type, predecessor>
    {
        typename kit_customizer<executor_type>::template result_type<value_type> result;
        ready_latch<predecessor> latch;
    };

    // void and one predecessor.
    template <typename executor_type, std::size_t predecessor>
    struct kit<executor_type, void, predecessor>
    {
        ready_latch<predecessor> latch;
    };

    // No predecessors: no need for a latch.
    template <typename executor_type, typename value_type>
    struct kit<executor_type, value_type>
    {
        typename kit_customizer<executor_type>::template result_type<value_type> result;
    };

    // Nothing.
    template <typename executor_type>
    struct kit<executor_type, void>
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
            using arg_predecessors = typename node_type::arg_predecessors::list;
            using await_predecessors = typename node_type::await_predecessors::list;
            using predecessors = mess::concatenate<arg_predecessors, await_predecessors>;

            return make_kit<executor_type, value_type>(predecessors());
        }

        template <typename executor_type, typename flat_graph, std::size_t... indexes>
        static constexpr decltype(std::make_tuple(make_kit<executor_type, invoke_result<flat_graph, indexes>, std::tuple_element_t<indexes, flat_graph>>()...)) make_runtime_state(std::index_sequence<indexes...>) noexcept
        {
            return {};
        }

        template <typename executor_type, typename flat_graph>
        using runtime_state = decltype(make_runtime_state<executor_type, flat_graph>(std::make_index_sequence<std::tuple_size_v<flat_graph>>()));
    } // namespace details
} // namespace mess
