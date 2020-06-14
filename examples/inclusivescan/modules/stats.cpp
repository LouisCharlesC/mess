/**
 * @file stats.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-02-24
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "stats.h"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/statistics/density.hpp>

namespace ex
{
	struct AccumulatorWrapper
	{
		boost::accumulators::accumulator_set<
			Stats::Duration::rep,
			boost::accumulators::stats<
				boost::accumulators::tag::max,
				boost::accumulators::tag::mean,
				boost::accumulators::tag::median,
				boost::accumulators::tag::min,
				boost::accumulators::tag::variance>> acc;
	};

	Stats::Stats():
		m_(new AccumulatorWrapper)
	{}
	Stats::~Stats()
	{
		delete m_;
	}

	void Stats::operator()(Duration duration)
	{
		m_->acc(duration.count());
	}

	Stats::Duration Stats::max() const
	{
		return Duration(static_cast<Duration::rep>(boost::accumulators::max(m_->acc)));
	}
	Stats::Duration Stats::mean() const
	{
		return Duration(static_cast<Duration::rep>(boost::accumulators::mean(m_->acc)));
	}
	Stats::Duration Stats::median() const
	{
		return Duration(static_cast<Duration::rep>(boost::accumulators::median(m_->acc)));
	}
	Stats::Duration Stats::min() const
	{
		return Duration(static_cast<Duration::rep>(boost::accumulators::min(m_->acc)));
	}
	Stats::Duration Stats::stddev() const
	{
		return Duration(std::sqrt(static_cast<Duration::rep>(boost::accumulators::variance(m_->acc))));
	}
} // namespace ex