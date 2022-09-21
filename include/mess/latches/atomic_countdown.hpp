/**
 * @file atomic_countdown.h
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <atomic>
#include <cstddef>

namespace mess
{
template <std::size_t... predecessors> class atomic_countdown_latch
{
  public:
    template <std::size_t notifying> bool notify_and_check_if_ready()
    {
        return --_pending == 0;
    }

  private:
    std::atomic<std::size_t> _pending = ATOMIC_VAR_INIT(sizeof...(predecessors));
};
} // namespace mess
