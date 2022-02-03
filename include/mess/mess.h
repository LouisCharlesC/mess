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
#include <optional>
#include <thread>
#include <vector>

namespace mess {
    
struct scope;

struct runtime
{
    struct countdown
    {
        signaler() = default;
        signaler(std::size_t numberOfPendingPredecessors, std::size_t taskIndex);
        signaler(const signaler& other);

        void signal(runtime& rt, scope& sc);

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
    static void execute(F&& f)
    {
        std::thread(std::forward<F>(f)).detach();
    }
};

inline int task(int l, int r)
{
    std::this_thread::sleep_for(std::chrono::milliseconds((l+r)*100)); return l+r;
}

struct scope
{

    runtime rt;
    std::vector<task> tasks{5};

    std::vector<runtime::signaler> signalers;
    std::vector<runtime::resource> ressources;

    void find_inputs_and_run(std::size_t index);
    void run(std::size_t index, const int& l, const int& r);

    int operator()(int l0, int r0, int l1, int r1);
};

inline void simple_invoker(std::size_t index, const int& l, const int& r)
{
    std::cout << "Starting task #" << index << "." << std::endl;
    const int result = task(l, r);
    std::cout << "Finished task #" << index << "." << std::endl;

    execute_successors(index, result, 0);
}

inline void execute_successors(std::size_t index, const int& l, const int& r)
{
    if (index == 0 || index >= 4)
    {
        execute_task(index+1, l, r);
    }
    else if (index == 1)
    {
        // no, no! scope here
        execute_task(2, l, r);
        execute_task(3, l, r);
    }
    else if (index == 2 || index == 3)
    {
        execute_task(4, l, r);
    }
}

inline void execute_task(std::size_t index, const int& l, const int& r)
{
    runtime::execute([&index, &l, &r](){invoke_task(index, l, r);});
}

inline void invoke_task(std::size_t index, const int& l, const int& r)
{
    if (index != 1)
    {
        simple_invoker(index, l, r);
    }
    else
    {
        scoped_invoker(index, l, r);
    }
}

inline int run(int i0)
{
    //     0
    //     |
    //     1
    //    / \
    //   2   3
    //    \ /
    //     4
    //     |
    //     5

    // add end task

    simple_runner(0, i0, 0)

    // await end task
}
} //namespace mess