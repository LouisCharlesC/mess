/**
 * @file launch.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-06-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <vector>

namespace mess
{
	using Task = int;
	using Tasks = std::vector<Task>;
	using Promise = char;
	using Promises = std::vector<Promise>;
	using Future = float;
	using Futures = std::vector<Future>;

	namespace impl
	{
		Tasks unique_children(Tasks tasks)
		{
			return {};
		}

		Tasks parents(Task task)
		{
			return {};
		}

		bool is_ready(Task task)
		{
			return {};
		}
	}

	namespace ext
	{
		Promises promises(Tasks task)
		{
			return {};
		}
		Futures futures(Promises promises)
		{
			return {};
		}
		Future execute(Task task)
		{
			return {};
		}
	}

	Futures launch(Tasks tasks)
	{
		Futures futures;

		Tasks children = impl::unique_children(tasks);
		for (Task& child: children)
		{
			thunk(child, futures);
		}

		return futures;
	}

	void thunk(Task task, Futures futures)
	{
		if (impl::unique_children({task}).empty())
		{
			ext::execute(task);
		}
		else
		{
			launch({task});
		}

		Tasks parents = impl::parents(task);
		for (Task parent: parents)
		{
			if (impl::is_ready(parent))
			{
				futures[parent] = ext::execute(parent);
			}
		}
	}
} // namespace mess