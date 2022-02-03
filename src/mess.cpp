/**
 * @file mess.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "mess/mess.h"

#include <iostream>

namespace mess {
    std_thread_runtime::signaler::signaler(std::size_t numberOfPendingPredecessors, std::size_t taskIndex):
        numberOfPendingPredecessors(numberOfPendingPredecessors), taskIndex(taskIndex)
    {}

    std_thread_runtime::signaler::signaler(const signaler& other): signaler(other.numberOfPendingPredecessors.load(), other.taskIndex) {}

    void std_thread_runtime::signaler::signal(std_thread_runtime& rt, scope& sc)
    {
        if (--numberOfPendingPredecessors == 0)
        {
            rt.execute([&sc, this](){sc.find_inputs_and_run(taskIndex);});
        }
    }

std_thread_runtime::resource std_thread_runtime::make_resource(signaler& first, signaler& second)
{
    return {std::nullopt, {&first, &second}};
}

std_thread_runtime::signaler std_thread_runtime::make_signaler(std::size_t numberOfPredecessors, std::size_t index)
{
    return {numberOfPredecessors, index};
}

void scope::find_inputs_and_run(std::size_t index)
{
    if (index == 2)
    {
        run(index, 0, *ressources[0].storage);
    }
    else if (index == 3)
    {
        run(index, *ressources[0].storage, *ressources[1].storage);
    }
    else if (index == 4)
    {
        run(index, *ressources[2].storage, *ressources[3].storage);
    }
    else if (index == 5)
    {
        run(index, *ressources[4].storage, *ressources[4].storage);
    }
}

void scope::run(std::size_t index, const int& l, const int& r)
{
    std::cout << "Starting task #" << index << "." << std::endl;
    ressources[index].storage.emplace(tasks[index].invocable(l, r));
    std::cout << "Finished task #" << index << "." << std::endl;

    for (auto& signaler: ressources[index].signalers)
    {
        signaler->signal(rt, *this);
    }
}

int scope::operator()(int l0, int r0, int l1, int r1)
{
    tasks[2].numberOfPredecessors = 1;

    std::atomic_flag flag;
    tasks.push_back(task{5, [&flag](int l, int r){flag.test_and_set(); flag.notify_one(); return l+r;}, {}});

    // allocate the signallers, one per task
    signalers.reserve(tasks.size()+1);
    for (const auto& task: tasks)
    {
        signalers.emplace_back(runtime::make_signaler(task.numberOfPredecessors, signalers.size())); // current signalers.size() is the task id.
    }

    // allocate results, provide each one their countdown
    ressources.reserve(tasks.size());
    ressources.emplace_back(runtime::make_resource(signalers[2], signalers[3])); // 0
    ressources.emplace_back(runtime::make_resource(signalers[3], signalers.back())); // 1
    ressources.emplace_back(runtime::make_resource(signalers.back(), signalers[4])); // 2
    ressources.emplace_back(runtime::make_resource(signalers[4], signalers.back())); // 3
    ressources.emplace_back(runtime::make_resource(signalers.back(), signalers.back())); // 4
    ressources.emplace_back(runtime::make_resource(signalers.back(), signalers.back())); // done

    // run stuff with rt.run(*this)
    rt.execute([this, &l0, &r0](){run(0, l0, r0);});
    rt.execute([this, &l1, &r1](){run(1, l1, r1);});

    // await done signal
    flag.wait(false);

    return *ressources.back().storage;
}
} //namespace mess
