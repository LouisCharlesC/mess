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
        // Pending threads can spawn new threads on their own, so we need to be careful.
        std::size_t pending = _pending;
        while (pending != 0) // if this number ever reached zero, we are done, since thee needs to be 1 pending thread to create new threads!
        {
            _pending.wait(pending); // Otherwise, wait for the value to change. Important to wait on `pending` (and not `_pending`) here because we know `pending`
            // is non-zero, while `_pending` could have changed to 0 in the meantime and we would wait forever!
            pending = _pending;
            // Then we will check if `pending == 0`. It does not matter that the value of _pending might have changed between the wait and the copy:
            // if it was zero after the wait, it cannot have changed. If it was non-zero and it changed to zero, then we will exit the loop, and if it was
            // and is still non-zero, we rinse and repeat.
        }
    }
} // namespace mess
