/**
 * @file concatenate.hpp
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
    template <std::size_t... lhs_indexes, std::size_t... rhs_indexes>
    constexpr std::index_sequence<lhs_indexes..., rhs_indexes...> operator+(std::index_sequence<lhs_indexes...>, std::index_sequence<rhs_indexes...>)
    {
        return {};
    }

    template <typename... sequences>
    using concatenate = decltype((sequences() + ...));
} // namespace mess