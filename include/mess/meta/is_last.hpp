/**
 * @file is_last.hpp
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
#include <type_traits>

namespace mess
{
    template <std::size_t index, std::size_t... indexes>
    static constexpr bool is_last(details::list<indexes...>)
    {
        // (indexes, ...) uses the comma operator to discard all indexes but the last.
        // Thanks https://www.foonathan.net/2020/05/fold-tricks/
        return index == std::integral_constant<std::size_t, (indexes, ...)>::value;
    }
} // namespace mess