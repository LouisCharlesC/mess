/**
 * @file inline.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <utility>

namespace mess
{
    constexpr struct inline_scheduler_t
    {
        template <typename F>
        constexpr void schedule(F &&f) const
        {
            std::forward<F>(f)();
        }

        inline constexpr void join() const noexcept {}
    } inline_scheduler;
} // namespace mess