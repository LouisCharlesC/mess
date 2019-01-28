/*
 * workerpool.h
 *
 *  Created on: 2017-04-28
 *      Author: lcc
 */

#ifndef WORKERPOOL_H_
#define WORKERPOOL_H_

#include "safe.hpp"

#include <algorithm>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

namespace mess {
	template<typename WorkQueueType>
	class WorkerPool
	{
    class Worker
    {
    public:
      Worker() = delete;
      Worker(WorkerPool& pool) noexcept:
        m_thread(&WorkerPool::workerWaitLoop, std::ref(pool))
      {}
      Worker(Worker&&) = default;
      Worker(std::thread&& thread):
        m_thread(std::move(thread))
      {}
      Worker& operator =(Worker&&) = default;
      ~Worker() noexcept
      {
        m_thread.join();
      }

    private:
      std::thread m_thread;
    };

    struct WorkQueueInfo
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
        safe::LockGuard<safe::Safe<WorkQueueInfo>>(m_pool.m_safeWorkQueueInfo)->queue.push(std::forward<Args>(args)...);
        m_pool.m_cv.notify_one();
      }

    private:
      WorkerPool& m_pool;
    };
    // friend WorkSubmitter;

    template<typename... WorkQueueConstructionArgs>
		WorkerPool(const std::uint_fast8_t& nbrOfWorkers, WorkQueueConstructionArgs&&... args) noexcept:
		  m_safeWorkQueueInfo(safe::default_construct_lockable, std::forward<WorkQueueConstructionArgs>(args)...),
		  m_submitter(*this)
		{
			m_safeWorkQueueInfo.unsafe().nbrOfBusyWorkers = nbrOfWorkers;
			m_workers.reserve(nbrOfWorkers);
			for (auto indexWorker = nbrOfWorkers; indexWorker != 0; --indexWorker)
			{
				m_workers.emplace_back(*this);
			}
		}

		~WorkerPool() noexcept
		{
      safe::LockGuard<safe::Safe<WorkQueueInfo>>(m_pool.m_safeWorkQueueInfo)->stop = true;
      m_cv.notify_all();
		}

		bool setWorkers(std::vector<std::thread>&& threads)
		{
		  bool success = false;

			if (m_workers.empty())
			{
				m_workers.reserve(threads.size());
				for (auto& thread: threads)
				{
					m_workers.push_back(std::move(thread));
				}
				threads.clear();

				success = true;
			}

			return success;
		}

		bool isWorkOngoing() const
		{
			return safe::LockGuard<safe::Safe<WorkQueueInfo>>(m_pool.m_safeWorkQueueInfo)->nbrOfBusyWorkers != 0;
		}

    WorkSubmitter& getWorkSubmitter()
    {
      return m_submitter;
    }

		void workerWaitLoop() noexcept
		{
			std::function<void()> work; // place-holder for piece of work to execute
			bool stop = false; // local copy of the shared stop flag
			while (!stop)
			{
				{ // acquire lock and wait for work
					auto workQueueInfo = safe::UniqueLock<safe::Safe<WorkQueueInfo>>(m_pool.m_safeWorkQueueInfo); // lock the work pool's mutex
					--workQueueInfo->nbrOfBusyWorkers;
					m_cv.wait(workQueueInfo.lock, [&workQueueInfo](){return workQueueInfo->stop || !workQueueInfo->queue.empty();});
					stop = workQueueInfo->stop;
					if (!stop) // if we are not asked to stop, the workQueue is necessarily not empty
					{
						// Fetch a piece of work
						++workQueueInfo->nbrOfBusyWorkers;
						workQueueInfo->queue.pop(work);
					}
				} // release mutex

				if (!stop) // if a piece of work was fetched
				{
					work(); // execute it
				}
			}
		}

	private:
		safe::Safe<WorkQueueInfo> m_safeWorkQueueInfo;
		std::condition_variable m_cv;
		std::vector<Worker> m_workers;
		WorkSubmitter m_submitter;
	};

	template<typename WorkQueueType>
	bool workerExecuteOne(safe::Safe<WorkQueueType>& safeWorkQueue) noexcept
	{
		bool empty = false;
		std::function<void()> work;
		{ // acquire lock and wait for work
			auto workQueue = safeWorkQueue.unique_lock(); // lock the work pool's mutex
			empty = workQueue->queue.empty();
			if (!empty)
			{
				// Fetch a piece of work
				workQueue->queue.pop(work);
			}
		} // release mutex

		if (!empty) // if a piece of work was fetched
		{
			work(); // execute it
		}
		return empty;
	}

	template<typename WorkQueueType>
	void workerExecuteAll(safe::Safe<WorkQueueType>& safeWorkQueue) noexcept
	{
		while (!workerExecuteOne(safeWorkQueue))
		{}
	}

}  // namespace mess

#endif /* WORKERPOOL_H_ */
