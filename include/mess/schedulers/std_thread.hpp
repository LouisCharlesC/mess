/**
 * @file std_thread.hpp
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
#include <thread>
#include <utility>

namespace mess
{
class std_thread_scheduler
{
  public:
    ~std_thread_scheduler() noexcept;

    template <typename F> void schedule(F f)
    {
        ++_pending;
        std::thread([f, this]() {
            f();
            --this->_pending;
            this->_pending.notify_one();
        }).detach();
    }

    void join() const noexcept;

  private:
    std::atomic<std::size_t> _pending = ATOMIC_VAR_INIT(0);
};
} // namespace mess
