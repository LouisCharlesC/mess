/*
 * workerpool.h
 *
 *  Created on: 2017-04-28
 *      Author: lcc
 */

#pragma once

#include "safe.hpp"

#include <algorithm>
#include <cassert>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

namespace mess {
	using ExecutableWork = std::function<void()>;

	template<typename WorkQueueType>
	class WorkerPool
	{
    class Worker
    {
    public:
      Worker() = delete;
      Worker(WorkerPool& pool) noexcept:
        m_thread(&WorkerPool::workerLoop, std::ref(pool))
      {}
      Worker(Worker&&) = default;
      Worker& operator =(Worker&&) = default;
      ~Worker() noexcept
      {
        m_thread.join();
      }

    private:
      std::thread m_thread;
    };

    struct WorkQueueAssets
    {
      WorkQueueType queue;
  		std::size_t nbrOfBusyWorkers = 0;
      bool stop = false;
    };

	public:
    class WorkSubmitter
    {
    public:
      WorkSubmitter(WorkerPool& pool):
        m_pool(pool)
      {}

      template<typename... Args>
      void operator()(Args&&... args)
      {
        m_pool.m_safeWorkQueue.writeAccess()->queue.push(std::forward<Args>(args)...);
        m_pool.m_workAvailable.notify_one();
      }

    private:
      WorkerPool& m_pool;
    };

    template<typename... WorkQueueConstructionArgs>
		WorkerPool(const std::uint_fast8_t& nbrOfWorkers, WorkQueueConstructionArgs&&... args) noexcept:
		  m_safeWorkQueue(safe::default_construct_lockable, std::forward<WorkQueueConstructionArgs>(args)...),
		  m_submitter(*this)
		{
			assert(nbrOfWorkers>0);

			m_safeWorkQueue.unsafe().nbrOfBusyWorkers = nbrOfWorkers;
			m_workers.reserve(nbrOfWorkers);
			for (auto indexWorker = nbrOfWorkers; indexWorker != 0; --indexWorker)
			{
				m_workers.emplace_back(*this);
			}
		}

		~WorkerPool() noexcept
		{
			m_safeWorkQueue.writeAccess()->stop = true;
			m_workAvailable.notify_all();
		}

		bool isWorkOngoing() const
		{
			auto&& workQueueAccess = m_safeWorkQueue.readAccess();
			return workQueueAccess->nbrOfBusyWorkers != 0;
		}

		// void waitStopped() const
		// {
		// 	auto workQueueAccess = m_safeWorkQueue.template readAccess<std::unique_lock>(); // lock the work queue's mutex
		// 	m_workOngoing.wait(workQueueAccess.lock, [this](){return !this->isWorkOngoing();});
		// }

    WorkSubmitter& getWorkSubmitter()
    {
      return m_submitter;
    }

		void workerLoop() noexcept
		{
			ExecutableWork work; // place-holder for piece of work to execute
			bool stop = false; // local copy of the shared stop flag

			while (!stop)
			{
				{
					auto workQueueAccess = m_safeWorkQueue.template writeAccess<std::unique_lock>(); // lock the work queue's mutex
					--workQueueAccess->nbrOfBusyWorkers;
					// m_workOngoing.notify_all();
					m_workAvailable.wait(workQueueAccess.lock, [&workQueueAccess](){return workQueueAccess->stop || !workQueueAccess->queue.empty();});
					stop = workQueueAccess->stop;
					if (!stop) // if we are not asked to stop, the workQueue is necessarily not empty
					{
						// Fetch a piece of work
						++workQueueAccess->nbrOfBusyWorkers;
						work = std::move(workQueueAccess->queue.top());
						workQueueAccess->queue.pop();
					}
				} // release mutex

				if (!stop) // if a piece of work was fetched
				{
					work(); // execute it
				}
			}

			m_workAvailable.notify_one();
		}

	private:
		safe::Safe<WorkQueueAssets> m_safeWorkQueue;
		std::condition_variable m_workAvailable;
		std::vector<Worker> m_workers;
		WorkSubmitter m_submitter;
		// std::condition_variable m_workOngoing;
	};
}  // namespace mess
