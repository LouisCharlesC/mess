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
#include <mess/meta/ordered_predecessors.hpp>
#include <mess/meta/sequences.hpp>

#include <cstdint>
#include <tuple>
#include <utility>

namespace mess
{
    template <typename scheduler_type, typename value_type, std::size_t... predecessors>
    struct kit
    {
        typename kit_customizer<scheduler_type>::template result_type<value_type> result;
        typename kit_customizer<scheduler_type>::template latch_type<predecessors...> latch;
    };

    // void: no need for a result.
    template <typename scheduler_type, std::size_t... predecessors>
    struct kit<scheduler_type, void, predecessors...>
    {
        typename kit_customizer<scheduler_type>::template latch_type<predecessors...> latch;
    };

    // One predecessor: use a ready latch.
    template <typename scheduler_type, typename value_type, std::size_t predecessor>
    struct kit<scheduler_type, value_type, predecessor>
    {
        typename kit_customizer<scheduler_type>::template result_type<value_type> result;
        ready_latch<predecessor> latch;
    };

    // void and one predecessor.
    template <typename scheduler_type, std::size_t predecessor>
    struct kit<scheduler_type, void, predecessor>
    {
        ready_latch<predecessor> latch;
    };

    // No predecessors: no need for a latch.
    template <typename scheduler_type, typename value_type>
    struct kit<scheduler_type, value_type>
    {
        typename kit_customizer<scheduler_type>::template result_type<value_type> result;
    };

    // Nothing.
    template <typename scheduler_type>
    struct kit<scheduler_type, void>
    {
    };

    namespace details
    {
        template <typename scheduler_type, typename flat_graph, typename value_type, std::size_t index>
        auto make_kit() noexcept
        {
            return []<std::size_t... predecessors>(std::index_sequence<predecessors...>)
            {
                return kit<scheduler_type, value_type, predecessors...>();
            }
            (mess::ordered_predecessors<flat_graph, index>());
        }

        template <typename scheduler_type, typename flat_graph, std::size_t... indexes>
        auto make_runtime_state(std::index_sequence<indexes...>) noexcept
        {
            return std::tuple<decltype(make_kit<scheduler_type, flat_graph, details::invoke_result<flat_graph, indexes>, indexes>())...>();
        }

        template <typename scheduler_type, typename flat_graph, std::size_t... indexes>
        auto make_self_delete_latch(std::index_sequence<indexes...>) noexcept
        {
            return []<std::size_t... leaf_nodes_index>(std::index_sequence<leaf_nodes_index...>)
            {
                return typename kit_customizer<scheduler_type>::template latch_type<leaf_nodes_index...>();
            }
            (leaf_nodes<flat_graph, indexes...>());
        }
    } // namespace details

    template <typename scheduler_type, typename flat_graph>
    using runtime_state = decltype(details::make_runtime_state<scheduler_type, flat_graph>(std::make_index_sequence<std::tuple_size_v<flat_graph>>()));

    template <typename scheduler_type, typename flat_graph>
    using self_delete_latch = decltype(details::make_self_delete_latch<scheduler_type, flat_graph>(details::ordered_graph<flat_graph>()));
} // namespace mess
