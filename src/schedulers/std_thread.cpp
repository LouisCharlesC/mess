/**
 * @file std_thread.cpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <mess/schedulers/std_thread.hpp>

namespace mess
{
    std_thread_scheduler::~std_thread_scheduler() noexcept
    {
        join();
    }

    void std_thread_scheduler::join() const noexcept
    {
        std::size_t pending = _pending;
        while (pending != 0)
        {
            _pending.wait(pending);
            pending = _pending;
        }
    }
} // namespace mess
