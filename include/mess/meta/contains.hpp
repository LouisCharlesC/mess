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

#include <mess/meta/list.hpp>

#include <cstdint>

namespace mess
{
    namespace details
    {
        template <std::size_t index, std::size_t... super_indexes>
        static constexpr bool contains()
        {
            return ((index == super_indexes) || ...);
        }

        template <std::size_t... indexes, std::size_t... super_indexes>
        static constexpr bool contains(list<indexes...>, list<super_indexes...>)
        {
            return (contains<indexes, super_indexes...> && ...);
        }
    } // namespace details

    template <typename list, typename super_list>
    using contains = decltype(details::contains(list(), super_list()));
} // namespace mess
