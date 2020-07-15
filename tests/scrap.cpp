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

// #include <mess/mess.h>

// #include <atomic>
#include <cassert>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <vector>
// #include <experimental/coroutine>

// namespace mess
// {
// 	class future{
// 	public:
// 		struct promise_type;
// 		using coro_handle = std::experimental::coroutine_handle<promise_type>;

// 		future(coro_handle handle) : handle_(handle) { assert(handle); }
// 		future(const future&) = delete;
// 		future(future&&) = delete;
// 		~future()
// 		{
// 			handle_.destroy();
// 		}

// 	bool resume()
// 	{
// 		if (not handle_.done())
// 		{
//       handle_.resume();
// 		}
//     return not handle_.done();
// 	}

// 	int get() const;

// 	private:
// 		coro_handle handle_;
// 	};

// 	struct future::promise_type
// 	{
// 		auto get_return_object()
// 		{
// 			return future::coro_handle::from_promise(*this);
// 		}

// 		auto initial_suspend()
// 		{
// 			return std::experimental::suspend_always();
// 		}

// 		auto final_suspend() noexcept
// 		{
// 			return std::experimental::suspend_always();
// 		}

// 		void return_value(int i) {_i = i;}

// 		void unhandled_exception()
// 		{
// 			std::terminate();
// 		}

// 		int _i;
// 	};

// 	int future::get() const
// 	{
// 		return handle_.promise()._i;
// 	}

// }

template<std::size_t N, typename... Args>
int func(Args&&... args)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(N*10));
	return (args + ... + N);
}

namespace linedep
{
	int func1()
	{
		return func<1>();
	}
	int func2(int _1)
	{
		return func<2>(_1);
	}
	int func3(int _2)
	{
		return func<3>(_2);
	}
	int func4(int _3)
	{
		return func<4>(_3);
	}
	int func5(int _4)
	{
		return func<5>(_4);
	}
}

// // 1 2 3 4 8
// //  5   6
// //    7   9
// //      10

namespace mess
{
	struct sequential
	{
		template<typename F, typename... Args>
		static auto launch(F&& f, Args&&... args)
		{
			return std::forward<F>(f)(std::forward<Args>(args)...);
		}

		template<typename Arg>
		static auto get(Arg&& arg)
		{
			return std::forward<Arg>(arg);
		}
	};

	// struct concurrent
	// {
	// 	template<typename F, typename... Args>
	// 	static mess::future launch(F&& f, Args&&... args)
	// 	{
	// 		// async gets
	// 		// co_await 
	// 		co_return //launch //sequential::launch(std::forward<F>(f), get(std::forward<Args>(args))...);
	// 	}

	// 	template<typename Arg>
	// 	static auto get(Arg&& arg)
	// 	{
	// 		// std::packaged_task<void()>([&arg](){arg.resume();})();
	// 		arg.resume();
	// 		return std::forward<Arg>(arg).get();
	// 	}
	// };
	struct parallel
	{
		template<typename F, typename... Args>
		static auto launch(F&& f, Args&&... args)
		{
			return std::async(
				[&f, &args...]
				()
				{
					return sequential::launch(std::forward<F>(f), get(std::forward<Args>(args))...);
				}).share();
		}

		template<typename Arg>
		static auto get(Arg&& arg)
		{
			return std::forward<Arg>(arg).get();
		}
	};
	// struct concurrent_async;
}

template<typename policy>
int sum()
{
	auto _1 = policy::launch([](){return func<1>();});
	auto _2 = policy::launch([](){return func<2>();});
	auto _3 = policy::launch([](){return func<3>();});
	auto _4 = policy::launch([](){return func<4>();});
	auto _8 = policy::launch([](){return func<8>();});

	auto _5 = policy::launch([](auto... args){return func<5>(args...);}, _1, _2);
	auto _6 = policy::launch([](auto... args){return func<6>(args...);}, _3, _4);

	auto _7 = policy::launch([](auto... args){return func<7>(args...);}, _5, _6);
	auto _9 = policy::launch([](auto... args){return func<9>(args...);}, _6, _8);

	return policy::get(policy::launch([](auto... args){return func<10>(args...);}, _7, _9));
	// with coro, we run off and delete everything.
}

// template<>
// int sum<mess::concurrent>()
// {
// 	auto _1 = mess::parallel::launch([](){return func<1>();});
// 	auto _2 = mess::parallel::launch([](){return func<2>();});
// 	auto _3 = mess::parallel::launch([](){return func<3>();});
// 	auto _4 = mess::parallel::launch([](){return func<4>();});
// 	auto _8 = mess::parallel::launch([](){return func<8>();});
// 	return EXIT_FAILURE;
// }

// bool ready(std::shared_future<int>& future)
// {
// 	return future.wait_for(std::chrono::steady_clock::duration::zero())==std::future_status::ready;
// }


template<>
int sum<mess::parallel>()
{

	auto f5 = [](int _4){return linedep::func5(_4);};
	auto f4 = [](int _3){return linedep::func4(_3);};
	auto f3 = [](int _2){return linedep::func3(_2);};
	// auto f2 = [](int _1){return linedep::func2(_1);};
	// auto f1 = [](){return linedep::func1();};

	auto ft4 =
		[&f4, &f5]
		(int _3)
		{
			// async call deps
			auto _4 = std::async([&f4, _3=std::move(_3)](){return f4(_3);});
			// async wait for deps and call func
			return std::async([&f5, _4=std::move(_4)](){return f5(_4.get());});
		};
	auto ft3 =
		[&f3, &ft4]
		(int _2)
		{
			// async call deps
			auto _3 = std::async([&f3, _2=std::move(_2)](){return f3(_2);});
			// async wait for deps and call func
			return std::async([&ft4, _3=std::move(_3)](){return ft4(_3.get());});
		};

	return ft3(2).get();
	// return _5.get();
}

int async(int);
int deps(int);

int frame(int curFuncs)
{
	// depFuncs = deps(curFuncs) + curPromises

	// cfutures=depFuncs+csigs async
	// 1 dep -> 1 sub-frame {...}

	// all ppromises=funcs+psigs(depfutures) async
	// func -> wait for depfutures, cfutures=func, signal psigs

	// wait for all funcs
}

enum class graph
{
	LineDep,
};
std::vector<int> getParentValue(graph dependencyGraph, int value)
{
	if (dependencyGraph == graph::LineDep)
	{
		return getLineDepParentFor(value);
	}
}
std::vector<int> getLineDepParentFor(int value)
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



// 

// template<>
// int sum<mess::concurrent_async>()
// { // 10
// 	std::function<int(int, int)> _10;
// 	{ // 7
// 		std::function<int()> _7;
// 		auto _5 = func<5>(1, 2);
// 		auto _6 = func<5>(3, 4);
// 		_7 = [&_5, &_6](){return func<7>(_5, _6);};
// 		_7();
// 	}
// 	auto _7 = mess::concurrent_async::launch([&_5, _6](){return func<7>(_5.get(), _6.get());});

	
// // 	auto _9 = mess::concurrent_async::launch([_6, &_8](){return func<9>(_6.get(), _8.get());});
// }

// int pool?()
// {
// 	auto _1 = std::async([](){return func<1>();});
// 	auto _2 = std::async([](){return func<2>();});
// 	auto _3 = std::async([](){return func<3>();});
// 	auto _4 = std::async([](){return func<4>();});
// 	auto _8 = std::async([](){return func<8>();});

// 	_1.wait(); _2.wait();
// 	auto _5 = std::async([&_1, &_2](){return func<5>(_1.get(), _2.get());});
// 	_3.wait(); _4.wait();
// 	std::shared_future<int> _6 = std::async([&_3, &_4](){return func<6>(_3.get(), _4.get());});
	
// 	_5.wait(); _6.wait();
// 	auto _7 = std::async([&_5, _6](){return func<7>(_5.get(), _6.get());});

// 	_6.wait(); _8.wait();
// 	auto _9 = std::async([_6, &_8](){return func<9>(_6.get(), _8.get());});

// 	_7.wait(); _9.wait();
// 	auto _10 = std::async([&_7, &_9](){return func<10>(_7.get(), _9.get());});

// 	return _10.get();
// }

int main()
{
	// {
	// 	const auto start = std::chrono::high_resolution_clock::now();

	// 	const auto answer = sum<mess::sequential>(); // 55 ms

	// 	const auto finish = std::chrono::high_resolution_clock::now();
	// 	const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count();
	// 	std::cout << "Waited " << elapsed << " ms to get answer: " << answer << "." << std::endl;
	// }

	{
		const auto start = std::chrono::high_resolution_clock::now();

		const auto answer = sum<mess::parallel>(); // 29 ms

		const auto finish = std::chrono::high_resolution_clock::now();
		const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count();
		std::cout << "Waited " << elapsed << " ms to get answer: " << answer << "." << std::endl;
	}

	// auto h = mess::concurrent::launch([](){std::cout << "Hello:"; return 10;});
	// h.resume();
	// h.resume();
	// std::cout << h.get() << std::endl;
	// std::cout << h.get() << std::endl;

	// {
	// 	const auto start = std::chrono::high_resolution_clock::now();

	// 	const auto answer = sum<mess::concurrent>(); // 29 ms

	// 	const auto finish = std::chrono::high_resolution_clock::now();
	// 	const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count();
	// 	std::cout << "Waited " << elapsed << " ms to get answer: " << answer << "." << std::endl;
	// }

	return EXIT_SUCCESS;
}