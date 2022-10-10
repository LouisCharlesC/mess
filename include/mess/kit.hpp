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
#include <mess/meta/disjunction.hpp>
#include <mess/meta/invoke_result.hpp>
#include <mess/meta/leaf_nodes.hpp>
#include <mess/meta/ordered_predecessors.hpp>
#include <mess/meta/sequences.hpp>

#include <cstddef>
#include <tuple>
#include <utility>

namespace mess
{
template <typename scheduler_type, typename value_type, std::size_t... predecessors> struct kit
{
    typename kit_customizer<scheduler_type>::template result_type<value_type> result;
    typename kit_customizer<scheduler_type>::template latch_type<predecessors...> latch;
};

// void: no need for a result.
template <typename scheduler_type, std::size_t... predecessors> struct kit<scheduler_type, void, predecessors...>
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
template <typename scheduler_type, std::size_t predecessor> struct kit<scheduler_type, void, predecessor>
{
    ready_latch<predecessor> latch;
};

// No predecessors: no need for a latch.
template <typename scheduler_type, typename value_type> struct kit<scheduler_type, value_type>
{
    typename kit_customizer<scheduler_type>::template result_type<value_type> result;
};

// Nothing.
template <typename scheduler_type> struct kit<scheduler_type, void>
{
};

namespace details
{
template <typename scheduler_type, typename flat_graph, typename value_type, std::size_t index> auto make_kit() noexcept
{
    return []<std::size_t... predecessors>(indexes<predecessors...>)
    {
        return kit<scheduler_type, value_type, predecessors...>();
    }
    (mess::ordered_predecessors<flat_graph, index>());
}

template <typename scheduler_type, typename flat_graph, std::size_t... all_indexes>
auto make_runtime_state(std::index_sequence<all_indexes...>) noexcept
{
    return std::tuple<decltype(make_kit<scheduler_type, flat_graph, details::invoke_result_t<flat_graph, all_indexes>,
                                        all_indexes>())...>();
}

template <typename scheduler_type, typename flat_graph, std::size_t... ordered_indexes>
auto make_self_delete_latch(indexes<ordered_indexes...>) noexcept
{
    return []<std::size_t... leaf_node_indexes>(indexes<leaf_node_indexes...>)
    {
        return typename kit_customizer<scheduler_type>::template latch_type<leaf_node_indexes...>();
    }
    (leaf_nodes<flat_graph, ordered_indexes...>());
}

template <typename cancel_flag_type, std::size_t index> struct indexed_cancel_flag
{
    cancel_flag_type flag;
};

template <typename scheduler_type, typename flat_graph, std::size_t... all_indexes>
auto make_cancel_flags(std::index_sequence<all_indexes...>) noexcept
{
    return []<std::size_t... leaf_node_indexes>(indexes<leaf_node_indexes...>)
    {
        return std::tuple<
            indexed_cancel_flag<typename kit_customizer<scheduler_type>::cancel_flag_type, leaf_node_indexes>...>();
    }
    (leaf_nodes<flat_graph, all_indexes...>());
}
} // namespace details

template <typename scheduler_type, typename flat_graph>
using runtime_state = decltype(details::make_runtime_state<scheduler_type, flat_graph>(
    std::make_index_sequence<std::tuple_size_v<flat_graph>>()));

template <typename scheduler_type, typename flat_graph>
using done_latch =
    decltype(details::make_self_delete_latch<scheduler_type, flat_graph>(details::ordered_indexes<flat_graph>()));

template <typename scheduler_type, typename flat_graph>
using cancel_flags = decltype(details::make_cancel_flags<scheduler_type, flat_graph>(
    std::make_index_sequence<std::tuple_size_v<flat_graph>>()));
} // namespace mess
