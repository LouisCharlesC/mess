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
#include <mess/meta/invoke_result.hpp>
#include <mess/meta/leaf_nodes.hpp>
#include <mess/meta/list.hpp>

#include <cstdint>
#include <tuple>
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

    namespace
    {
        template <typename executor_type, typename value_type, std::size_t... predecessors>
        auto make_kit(details::list<predecessors...>) noexcept
        {
            return kit<executor_type, value_type, predecessors...>();
        }
        template <typename executor_type, typename value_type, typename node_type>
        auto make_kit() noexcept
        {
            return make_kit<executor_type, value_type>(typename node_type::all_predecessors());
        }

        template <typename executor_type, typename flat_graph, std::size_t... indexes>
        auto make_runtime_state(std::index_sequence<indexes...>) noexcept
        {
            return std::tuple<decltype(make_kit<executor_type, details::invoke_result<flat_graph, indexes>, std::tuple_element_t<indexes, flat_graph>>())...>();
        }

        template <typename executor_type, std::size_t... indexes>
        auto make_self_delete_latch(details::list<indexes...>) noexcept
        {
            return typename kit_customizer<executor_type>::template latch_type<indexes...>();
        }

        template <typename executor_type, typename flat_graph, std::size_t... indexes>
        auto make_self_delete_latch(std::index_sequence<indexes...>) noexcept
        {
            return make_self_delete_latch<executor_type>(leaf_nodes<flat_graph, indexes...>());
        }
    } // namespace

    template <typename executor_type, typename flat_graph>
    using runtime_state = decltype(make_runtime_state<executor_type, flat_graph>(std::make_index_sequence<std::tuple_size_v<flat_graph>>()));

    template <typename executor_type, typename flat_graph>
    using self_delete_latch = decltype(make_self_delete_latch<executor_type, flat_graph>(std::make_index_sequence<std::tuple_size_v<flat_graph>>()));
} // namespace mess
