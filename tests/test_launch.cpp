/**
 * @file test_lunch.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-06-25
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <algorithm>
#include <cassert>
#include <chrono>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

std::size_t func(std::size_t N, std::vector<std::size_t> args)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(N*10));
	return (std::accumulate(args.cbegin(), args.cend(), N));
}

enum class graph
{
	LineDep,
	VDep,
};
std::vector<std::size_t> getLineDepParentsFor(std::size_t value)
{
	if (value == 0)
	{
		return {};
	}
	else
	{
		return {value-1};
	}
}
std::vector<std::size_t> getVDepParentsFor(std::size_t value)
{
	switch (value)
	{
		case 1:
			return {};
		case 2:
			return {};
		case 3:
			return {};
		case 4:
			return {};
		case 5:
			return {1, 2};
		case 6:
			return {3, 4};
		case 7:
			return {5, 6};
		case 8:
			return {};
		case 9:
			return {6, 8};
		case 10:
			return {7, 9};
	}
	return {};
}
std::vector<std::size_t> getParentsFor(graph dependencyGraph, std::size_t value)
{
	if (dependencyGraph == graph::LineDep)
	{
		return getLineDepParentsFor(value);
	}
	else if (dependencyGraph == graph::VDep)
	{
		return getVDepParentsFor(value);
	}
	return {};
}
std::size_t getAnswerFor(graph dependencyGraph, std::size_t value)
{
	assert(value==10);
	if (dependencyGraph == graph::LineDep)
	{
		return 55;
	}
	else if (dependencyGraph == graph::VDep)
	{
		return 68;
	}
	return 0;
}
std::size_t getElapsedFor(graph dependencyGraph, std::size_t value)
{
	assert(value==10);
	if (dependencyGraph == graph::LineDep)
	{
		return getAnswerFor(graph::LineDep, value);
	}
	else if (dependencyGraph == graph::VDep)
	{
		return 40;
	}
	return 0;
}

namespace mess
{
	struct sequential;;
	// struct concurrent;
	struct parallel;
	// struct concurrent_async;
}

template<typename policy>
std::size_t sum(graph dependencyGraph, std::size_t N);

template<>
std::size_t sum<mess::sequential>(graph dependencyGraph, std::size_t N)
{
	const auto deps = getParentsFor(dependencyGraph, N);

	std::vector<std::size_t> funcdeps(deps.size());
	std::transform(deps.cbegin(), deps.cend(), funcdeps.begin(),
		[&dependencyGraph]
		(std::size_t N)
		{
			return sum<mess::sequential>(dependencyGraph, N);
		});

	return func(N, funcdeps);
}

template<>
std::size_t sum<mess::parallel>(graph dependencyGraph, std::size_t N)
{
	const auto deps = getParentsFor(dependencyGraph, N);

	std::vector<std::future<std::size_t>> futdeps(deps.size());
	std::transform(deps.cbegin(), deps.cend(), futdeps.begin(),
		[&dependencyGraph]
		(std::size_t N)
		{
			return std::async(
				[&dependencyGraph, N]
				()
				{
					return sum<mess::sequential>(dependencyGraph, N);
				});
		});
	std::vector<std::size_t> funcdeps(futdeps.size());
	std::transform(futdeps.begin(), futdeps.end(), funcdeps.begin(),
		[]
		(std::future<std::size_t>& fut)
		{
			return fut.get();
		});

	return func(N, funcdeps);
}

template<typename Exec>
void measure(graph graph)
{
	constexpr std::size_t value = 10;

	const auto start = std::chrono::high_resolution_clock::now();

	const auto answer = sum<Exec>(graph, value);

	const auto finish = std::chrono::high_resolution_clock::now();
	const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count();
	assert(answer == getAnswerFor(graph, value));
	assert((elapsed-5)/10 == getElapsedFor(graph, value));
	std::cout << "Waited " << elapsed << " ms to get answer: " << answer << "." << std::endl;
}

int main()
{
	constexpr auto graph = graph::VDep;
	measure<mess::sequential>(graph);
	measure<mess::parallel>(graph);

	return EXIT_SUCCESS;
}