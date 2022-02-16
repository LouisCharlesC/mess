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

namespace mess
{
    namespace details
    {
        template <typename flat_graph, std::size_t index, std::size_t... arg_predecessors_index>
        static constexpr auto get_invoke_result(mess::arg_predecessors<arg_predecessors_index...>);

        template <typename flat_graph, std::size_t index>
        using invoke_result = decltype(get_invoke_result<flat_graph, index>(typename std::tuple_element_t<index, flat_graph>::arg_predecessors()));

        template <typename flat_graph, std::size_t index, std::size_t... arg_predecessors_index>
        static constexpr auto get_invoke_result(mess::arg_predecessors<arg_predecessors_index...>)
        {
            return std::invoke_result_t<typename std::tuple_element_t<index, flat_graph>::invocable_type, invoke_result<flat_graph, arg_predecessors_index>...>{};
        }
    } // namespace details
} // namespace mess