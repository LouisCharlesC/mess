/**
 * @file stats.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-02-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include <chrono>
#include <cmath>

namespace ex
{
	struct AccumulatorWrapper;

	class Stats
	{
	public:
		using Duration = std::chrono::duration<double, std::milli>;

		Stats();
		~Stats();

		void operator()(Duration duration);

		Duration max() const;
		Duration mean() const;
		Duration median() const;
		Duration min() const;
		Duration stddev() const;

	private:
		AccumulatorWrapper* m_;
	};
} // namespace ex