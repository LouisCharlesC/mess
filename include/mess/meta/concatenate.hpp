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
    namespace details
    {
        template <typename list>
        static consteval auto concatenate(list)
        {
            return list();
        }

        template <std::size_t... first_indexes, std::size_t... second_indexes>
        static constexpr auto concatenate(list<first_indexes...>, list<second_indexes...>)
        {
            return list<first_indexes..., second_indexes...>();
        }

        template <typename first_list, typename second_list, typename third_list, typename... other_lists>
        static constexpr auto concatenate(first_list, second_list, third_list, other_lists...)
        {
            return concatenate(concatenate(first_list(), second_list()), third_list(), other_lists()...);
        }
    } // namespace details

    template <typename... lists>
    using concatenate = decltype(details::concatenate(lists()...));
} // namespace mess