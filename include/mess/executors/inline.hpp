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
    struct inline_executor
    {
        template <typename F>
        inline constexpr void execute(F &&f) const
        {
            std::forward<F>(f)();
        }

        inline constexpr void join() const noexcept {}
    };
} // namespace mess