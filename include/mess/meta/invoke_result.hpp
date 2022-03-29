/**
 * @file invoke_result.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <mess/meta/sequences.hpp>

#include <cstdint>
#include <tuple>
#include <type_traits>
#include <utility>

namespace mess
{
    namespace details
    {
        // We need a template struct specialization to get a parameter pack, so first declare the template.
        template <typename flat_graph, std::size_t index, typename>
        struct invoke_result;
        // And forward declare the specialization we need.
        template <typename flat_graph, std::size_t index, std::size_t... arg_predecessors_index>
        struct invoke_result<flat_graph, index, mess::arg_predecessors<arg_predecessors_index...>>;

        // Convenience alias to the type defined in the specialization.
        template <typename flat_graph, std::size_t index>
        using invoke_result_t = typename invoke_result<flat_graph, index, typename std::tuple_element_t<index, flat_graph>::arg_predecessors>::type;

        // The actual definition of the specialization.
        template <typename flat_graph, std::size_t index, std::size_t... arg_predecessors_index>
        struct invoke_result<flat_graph, index, mess::arg_predecessors<arg_predecessors_index...>>
        {
            // Recursevely get the invoke result of the predecessors.
            using type = std::invoke_result_t<typename std::tuple_element_t<index, flat_graph>::invocable_type, invoke_result_t<flat_graph, arg_predecessors_index>...>;
        };
    } // namespace details
} // namespace mess