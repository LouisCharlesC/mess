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

#include <mess/meta/is_last.hpp>
#include <mess/meta/list.hpp>

#include <cstdint>

namespace mess
{
    template <std::size_t... predecessors>
    struct last_predecessor_latch
    {
        template <std::size_t notifying>
        constexpr bool notify_and_check_if_ready() const noexcept
        {
            return is_last<notifying>(list<predecessors...>());
        }
    };
} // namespace mess