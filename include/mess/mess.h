/**
 * @file mess.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-12-02
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <utility>
#include <thread>
#include <vector>

namespace mess
{
    struct entry
    {
        std::function<int(int, int)> invocable;
        std::vector<int> predecessors_index;
        int count_down_latch;
        std::vector<int> successors_index;
    };

    struct frame
    {
        std::vector<mess::entry> descriptor;
        std::vector<int> results;
    };

    inline void execute(frame &frame, std::size_t index);
    inline void run(frame &frame, std::size_t index)
    {
        int lhs = 1;
        int rhs = 1;
        if (frame.descriptor[index].predecessors_index.size() > 0)
        {
            lhs = frame.results[frame.descriptor[index].predecessors_index[0]];

            if (frame.descriptor[index].predecessors_index.size() > 1)
            {
                rhs = frame.results[frame.descriptor[index].predecessors_index[1]];
            }
        }

        if (!frame.descriptor[index].successors_index.empty())
        {
            frame.results[index] = frame.descriptor[index].invocable(lhs, rhs);

            const auto successors = frame.descriptor[index].successors_index;
            for (const int successor : successors)
            {
                if (--frame.descriptor[successor].count_down_latch == 0)
                {
                    execute(frame, successor);
                }
            }
        }
        else
        {
            frame.descriptor[index].invocable(lhs, rhs);
        }
    }

    inline void execute(frame &frame, std::size_t index)
    {
        std::thread([&frame, index]()
                    { run(frame, index); })
            .detach();
        // run(frame, index);
    }

    inline frame make_frame(const std::vector<mess::entry> &descriptor)
    {
        return {descriptor, std::vector<int>(descriptor.size())};
    }

    inline void run(const std::vector<mess::entry> &descriptor)
    {
        frame frame = make_frame(descriptor);

        execute(frame, 0);
    }

    inline void run(std::unique_ptr<frame> ptr)
    {
        mess::frame &frame = *ptr;

        frame.descriptor.back().successors_index.push_back(frame.descriptor.size());
        frame.descriptor.push_back(mess::entry{[ptr = std::shared_ptr(std::move(ptr))](int, int) mutable -> int
                                               {
                                                   std::shared_ptr release(std::move(ptr));
                                                   return 0;
                                               },
                                               {},
                                               1,
                                               {}});

        for (std::size_t i = 0; i < frame.descriptor.size(); ++i)
        {
            if (frame.descriptor[i].count_down_latch == 0)
            {
                execute(frame, i);
            }
        }
    }
} // namespace mess
