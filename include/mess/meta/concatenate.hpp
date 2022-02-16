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

#include <mess/meta/list.hpp>

#include <cstdint>

namespace mess
{
    template <std::size_t... first_indexes, std::size_t... second_indexes>
    static constexpr details::list<first_indexes..., second_indexes...> operator+(details::list<first_indexes...>, details::list<second_indexes...>)
    {
        return {};
    }

    template <typename... lists>
    using concatenate = decltype((lists() + ...));
} // namespace mess