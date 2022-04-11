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
    template <std::size_t... indexes>
    struct mess_sequence
    {
        using std_sequence = std::index_sequence<indexes...>;

        mess_sequence(std::index_sequence<indexes...>);
    };

    template <std::size_t... lhs_indexes, std::size_t... rhs_indexes>
    constexpr mess_sequence<lhs_indexes..., rhs_indexes...> operator+(mess_sequence<lhs_indexes...>, mess_sequence<rhs_indexes...>)
    {
        return {};
    }

    template <typename... sequences>
    using concatenate = typename decltype((mess_sequence(sequences()) + ...))::std_sequence;
} // namespace mess