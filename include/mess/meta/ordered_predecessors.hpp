/**
 * @file ordered_predecessors.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <mess/meta/concatenate.hpp>
#include <mess/meta/contains.hpp>
#include <mess/meta/root_nodes.hpp>

#include <cstdint>
#include <utility>
#include <tuple>
#include <type_traits>

namespace mess
{
    namespace details
    {
        template <typename flat_graph, std::size_t... ordered, std::size_t first, std::size_t... others>
        consteval auto recurse_on_siblings(std::index_sequence<ordered...>, std::index_sequence<first, others...>);

        template <typename flat_graph, std::size_t current, std::size_t... ordered>
        consteval auto recurse_on_successors_if_ready(std::index_sequence<ordered...>)
        {
            using predecessors = typename std::tuple_element_t<current, flat_graph>::unordered_predecessors;
            constexpr bool is_ready = mess::contains<predecessors, std::index_sequence<ordered...>>;

            if constexpr (is_ready)
            {
                using successors = typename std::tuple_element_t<current, flat_graph>::successors;
                constexpr auto ordered_with_current = std::index_sequence<ordered..., current>();

                if constexpr (successors::size() == 0)
                {
                    return ordered_with_current;
                }
                else
                {
                    return recurse_on_siblings<flat_graph>(ordered_with_current, successors());
                }
            }
            else
            {
                return std::index_sequence<ordered...>();
            }
        }

        template <typename flat_graph, std::size_t... ordered, std::size_t first, std::size_t... others>
        consteval auto recurse_on_siblings(std::index_sequence<ordered...>, std::index_sequence<first, others...>)
        {
            constexpr auto ordered_successors = recurse_on_successors_if_ready<flat_graph, first>(std::index_sequence<ordered...>());

            if constexpr (sizeof...(others) == 0)
            {
                return ordered_successors;
            }
            else
            {
                return recurse_on_siblings<flat_graph>(ordered_successors, std::index_sequence<others...>());
            }
        }

        template <typename flat_graph>
        consteval auto ordered_graph()
        {
            return []<std::size_t... indexes>(std::index_sequence<indexes...>)
            {
                return recurse_on_siblings<flat_graph>(std::index_sequence<>(), root_nodes<flat_graph, indexes...>());
            }
            (std::make_index_sequence<std::tuple_size_v<flat_graph>>());
        }

        template <typename flat_graph, std::size_t index, std::size_t... ordered>
        consteval auto ordered_predecessors(std::index_sequence<ordered...>)
        {
            using predecessors = typename std::tuple_element_t<index, flat_graph>::unordered_predecessors;
            return concatenate<std::conditional_t<contains(std::index_sequence<ordered>(), predecessors()),
                                                  std::index_sequence<ordered>,
                                                  std::index_sequence<>>...>();
        }
    } // namespace details

    template <typename flat_graph, std::size_t index>
    using ordered_predecessors = decltype(details::ordered_predecessors<flat_graph, index>(details::ordered_graph<flat_graph>()));
} // namespace mess
