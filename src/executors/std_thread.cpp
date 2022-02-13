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

#include <mess/executors/std_thread.hpp>

namespace mess
{
    std_thread_executor::~std_thread_executor() noexcept
    {
        join();
    }

    void std_thread_executor::join() const noexcept
    {

        std::size_t running = _running;
        while (running != 0)
        {
            _running.wait(running);
            running = _running;
        }
    }
} // namespace mess
