/**
 * @file frameandthunk.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <cassert>
#include <future>
#include <map>
#include <numeric>
#include <vector>

class Task
{
public:
	using Id = int;
	using Promise = std::promise<int>;
	using Future = std::future<int>;

	Task(Id id):
		_id(id)
	{}

	static int invoke(Id id, const std::vector<int>& args)
	{
		return std::accumulate(args.cbegin(), args.cend(), id, std::plus());
	}

	Id id() const {return _id;}

private:
	Id _id;
};

class Graph
{
	using Map = std::map<Task::Id, std::vector<Task::Id>>;

public:

	Graph(Map predecessors, Map successors, Map scoped_successors):
		_predecessors(predecessors),
		_successors(successors),
		_scoped_successors(scoped_successors)
	{}

	const std::vector<Task::Id>& predecessors(Task::Id id) const
	{
		return _predecessors.at(id);
	}
	const std::vector<Task::Id>& successors(Task::Id id) const
	{
		return _successors.at(id);
	}
	const std::vector<Task::Id>& scoped_successors(Task::Id id) const
	{
		return _scoped_successors.at(id);
	}

private:
	Map _predecessors;
	Map _successors;
	Map _scoped_successors;
};

struct Execution;
int pull(const Task::Id task, const std::vector<int>& args, const Execution& exe, const Graph& graph, std::map<const Task::Id, Task::Promise> promises, std::map<const Task::Id, Task::Future> futures);

struct Execution
{
	struct Future
	{
		void wait() const {}
	};

	Future exec(const Task::Id task, const Graph& graph, std::map<Task::Id, Task::Promise*> promises, std::map<Task::Id, Task::Future*> futures) const
	{
		std::vector<int> args;
		// Direct predecessors
		for (Task::Id id: graph.predecessors(task))
		{
			args.push_back(futures[id]->get());
		}

		pull(task, args, *this, graph, promises, futures);

		return {};
	}
};

int pull(const Task::Id task, const std::vector<int>& args, const Execution& exe, const Graph& graph, std::map<Task::Id, Task::Promise*> promises, std::map<Task::Id, Task::Future*> futures)
{
	// The current task.
	Task::Promise maybe_promise;
	Task::Future maybe_future;
	Task::Promise* task_promise;
	if (promises.contains(task) && futures.contains(task))
	{
		task_promise = promises.extract(task).mapped();
	}
	else
	{
		assert(!promises.contains(task) && !futures.contains(task));
		maybe_future = maybe_promise.get_future();

		task_promise = &maybe_promise;
		futures[task] = &maybe_future;
	}
	task_promise->set_value(Task::invoke(task, args));

	// Execute direct successors
	std::vector<Execution::Future> execution_futures;
	for (Task::Id id: graph.successors(task))
	{
		execution_futures.push_back(exe.exec(id, graph, promises, futures));
	}

	// Promises and futures scoped by this task.
	std::map<Task::Id, Task::Future> scoped_futures;
	std::map<Task::Id, Task::Promise> scoped_promises;
	// Populate
	for (Task::Id scoped: graph.scoped_successors(task))
	{
		for (Task::Id pred: graph.predecessors(scoped))
		{
			assert(!promises.contains(pred) && !futures.contains(pred));
			scoped_futures[pred] = scoped_promises[pred].get_future();
			promises.insert({pred, &scoped_promises[pred]});
			futures.insert({pred, &scoped_futures[pred]});
		}
	}

	// Execute scoped successors
	for (Task::Id scoped: graph.scoped_successors(task))
	{
		execution_futures.push_back(exe.exec(scoped, graph, promises, futures));
	}

	// Wait for dependent tasks to finish
	for (Execution::Future& future: execution_futures)
	{
		future.wait();
	}

	// Clean-up
	futures.extract(task);
	// Check clean-up done by dependant tasks
	for (Task::Id id: graph.successors(task))
	{
		assert(!promises.contains(id) && !futures.contains(id));
	}
	for (Task::Id id: graph.scoped_successors(task))
	{
		assert(!promises.contains(id) && !futures.contains(id));
	}

	// Return the task's output
	return futures[task]->get();
}

int main()
{


	
    return 1;
}