/*
 * workerpool.h
 *
 *  Created on: 2017-04-28
 *      Author: lcc
 */

#pragma once

#include "safe/safe.h"

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

		enum class WorkQueueState
		{
			Run,
			StopWhenEmpty,
			StopNow
		};

		struct WorkQueueAssets
		{
			WorkQueueType queue;
			WorkQueueState state = WorkQueueState::Run;
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

			m_workers.reserve(nbrOfWorkers);
			for (auto indexWorker = nbrOfWorkers; indexWorker != 0; --indexWorker)
			{
				m_workers.emplace_back(*this);
			}
		}

		~WorkerPool()
		{
			stop();
		}

		WorkSubmitter& getWorkSubmitter()
		{
			return m_submitter;
		}

		void stop()
		{
			m_safeWorkQueue.writeAccess()->state = WorkQueueState::StopNow;
			m_workAvailable.notify_all();
			joinWorkers();
		}

		void emptyAndStop()
		{
			m_safeWorkQueue.writeAccess()->state = WorkQueueState::StopWhenEmpty;
			m_workAvailable.notify_all();
			joinWorkers();
		}

		void workerLoop() noexcept
		{
			ExecutableWork work; // place-holder for piece of work to execute
			bool stop = false; // local copy of the shared stop flag

			while (!stop)
			{
				{
					auto workQueueAccess = m_safeWorkQueue.template writeAccess<std::unique_lock>(); // lock the work queue's mutex
					m_workAvailable.wait(workQueueAccess.lock, [&workQueueAccess](){return isAskedToStop(*workQueueAccess) || !workQueueAccess->queue.empty();});
					stop = isAskedToStop(*workQueueAccess);
					if (!stop) // if we are not asked to stop, the workQueue is necessarily not empty
					{
						// Fetch a piece of work
						work = std::move(workQueueAccess->queue.top());
						workQueueAccess->queue.pop();
					}
				} // release mutex

				if (!stop) // if a piece of work was fetched
				{
					work(); // execute it
				}
			}
		}

	private:
		static bool isAskedToStop(const WorkQueueAssets& workQueue)
		{
			return workQueue.state == WorkQueueState::StopNow || (workQueue.state == WorkQueueState::StopWhenEmpty && workQueue.queue.empty());;
		}

		void joinWorkers()
		{
			m_workers.clear();
		}

		safe::Safe<WorkQueueAssets> m_safeWorkQueue;
		std::condition_variable m_workAvailable;
		std::vector<Worker> m_workers;
		WorkSubmitter m_submitter;
	};
}  // namespace mess
