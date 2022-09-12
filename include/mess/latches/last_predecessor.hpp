/**
 * @file last_predecessor.h
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-05
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <cstdint>

namespace mess
{
template <std::size_t... predecessors> struct last_predecessor_latch
{
    template <std::size_t notifying> constexpr bool notify_and_check_if_ready() const noexcept
    {
        return is_last<notifying>;
    }

  private:
    // (indexes, ...) uses the comma operator to discard all indexes but the last.
    // Thanks https://www.foonathan.net/2020/05/fold-tricks/
    template <std::size_t index> static constexpr bool is_last = index == (predecessors, ...);
};
} // namespace mess