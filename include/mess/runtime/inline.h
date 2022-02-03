/**
 * @file inline.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "mess/mess.h"

namespace mess {
struct inline_runtime
{
    struct signaler
    {
        signaler() = default;
        signaler(std::size_t numberOfPendingPredecessors, std::size_t taskIndex);
        signaler(const signaler& other);

        void signal(std_thread_runtime& rt, scope& sc);

        std::atomic<std::size_t> numberOfPendingPredecessors;
        std::size_t taskIndex;
    };

    struct resource
    {
        std::optional<int> storage;
        std::array<signaler*, 2> signalers;
    };

    static resource make_resource(signaler& first, signaler& second);

    static signaler make_signaler(std::size_t numberOfPredecessors, std::size_t index);

    template<typename F>
    void execute(F&& f)
    {
        std::forward<F>(f)();
    }
};
} //namespace mess