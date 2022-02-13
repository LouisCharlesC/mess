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
#include <cstdint>
#include <thread>
#include <utility>

namespace mess
{
    class std_thread_executor
    {
    public:
        ~std_thread_executor() noexcept;

        template <typename F>
        void execute(F f)
        {
            ++_running;
            std::thread([f, this]()
                        {
                            f();
                            --this->_running;
                            this->_running.notify_one(); })
                .detach();
        }

        void join() const noexcept;

    private:
        std::atomic<std::size_t> _running = ATOMIC_VAR_INIT(0);
    };
} // namespace mess
