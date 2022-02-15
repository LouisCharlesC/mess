/**
 * @file list.hpp
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

namespace mess
{
    namespace details
    {
        template <std::size_t... indexes>
        struct list
        {
            static constexpr std::size_t count = sizeof...(indexes);
            static constexpr bool empty = count == 0;
        };
    } // namespace details

    template <std::size_t... indexes>
    struct arg_predecessors : details::list<indexes...>
    {
        using list = details::list<indexes...>;
    };
    template <std::size_t... indexes>
    struct await_predecessors : details::list<indexes...>
    {
        using list = details::list<indexes...>;
    };
    template <std::size_t... indexes>
    struct successors : details::list<indexes...>
    {
        using list = details::list<indexes...>;
    };

    template <std::size_t... indexes>
    constexpr bool is_arg_predecessors_type(arg_predecessors<indexes...>) { return true; }
    template <std::size_t... indexes>
    constexpr bool is_await_predecessors_type(await_predecessors<indexes...>) { return true; }
    template <std::size_t... indexes>
    constexpr bool is_successors_type(successors<indexes...>) { return true; }
} // namespace mess