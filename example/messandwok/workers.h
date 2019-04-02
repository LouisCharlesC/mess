/**
 * @file workers.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "workerpool.h"

#include <queue>

class WorkQueue
{
public:
	bool empty() const
	{
		return m_queue.empty();
	}
	template<typename... Args>
	void push(Args&&... args)
	{
		m_queue.push(std::forward<Args>(args)...);
	}
	mess::ExecutableWork& top()
	{
		return m_queue.front();
	}
	void pop()
	{
		m_queue.pop();
	}

private:
	std::queue<mess::ExecutableWork> m_queue;
};

using Workers = mess::WorkerPool<WorkQueue>;