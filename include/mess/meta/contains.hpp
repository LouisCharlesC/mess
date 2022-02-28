/**
 * @file contains.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <cstdint>
#include <utility>

namespace mess
{
    namespace details
    {
        template <std::size_t index, std::size_t... super_indexes>
        consteval bool contains()
        {
            return ((index == super_indexes) || ...);
        }

        template <std::size_t... indexes, std::size_t... super_indexes>
        consteval bool contains(std::index_sequence<indexes...>, std::index_sequence<super_indexes...>)
        {
            return (contains<indexes, super_indexes...>() && ...);
        }
    } // namespace details

    template <typename sequence, typename super_sequence>
    constexpr bool contains = details::contains(sequence(), super_sequence());
} // namespace mess
