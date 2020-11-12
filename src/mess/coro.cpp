/**
 * @file main.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-10-27
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <experimental/coroutine>

#include <cassert>
#include <iostream>

namespace mess
{
	class future{
	public:
		struct promise_type;
		using coro_handle = std::experimental::coroutine_handle<promise_type>;

		future(coro_handle handle) : handle_(handle) { assert(handle); }
		future(const future&) = delete;
		future(future&&) = delete;
		~future()
		{
			handle_.destroy();
		}

	bool resume()
	{
		if (not handle_.done())
		{
      handle_.resume();
		}
    return not handle_.done();
	}

	int get() const;

	private:
		coro_handle handle_;
	};

	struct future::promise_type
	{
		auto get_return_object()
		{
			return future::coro_handle::from_promise(*this);
		}

		auto initial_suspend()
		{
			return std::experimental::suspend_always();
		}

		auto final_suspend() noexcept
		{
			return std::experimental::suspend_always();
		}

		void return_value(int i) {_i = i;}

		void unhandled_exception()
		{
			std::terminate();
		}

		int _i;
	};

	int future::get() const
	{
		return handle_.promise()._i;
	}
}

	mess::future launch()
	{
		co_return EXIT_SUCCESS;
	}

int main()
{
	// c'est pas comme ça que ça s'utilise, mais bon.
	launch();
	return launch().get();
}