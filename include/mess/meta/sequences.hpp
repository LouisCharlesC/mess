/**
 * @file sequences.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <cstdint>
#include <utility>

namespace mess
{
    template <std::size_t... indexes>
    struct arg_predecessors : std::index_sequence<indexes...>
    {
    };
    template <std::size_t... indexes>
    struct other_predecessors : std::index_sequence<indexes...>
    {
    };
    template <std::size_t... indexes>
    struct successors : std::index_sequence<indexes...>
    {
    };

    template <std::size_t... indexes>
    constexpr bool is_arg_predecessors_type(arg_predecessors<indexes...>) { return true; }
    template <std::size_t... indexes>
    constexpr bool is_other_predecessors_type(other_predecessors<indexes...>) { return true; }
    template <std::size_t... indexes>
    constexpr bool is_successors_type(successors<indexes...>) { return true; }
} // namespace mess