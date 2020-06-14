/**
 * @file sorter.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-11-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <iterator>
#include <numeric>

namespace ex
{
	// template<typename ValueType>
	// struct ScanOperation
	// {
	// 	constexpr ValueType operator()(ValueType lhs, ValueType rhs)
	// 	{
	// 		return lhs+std::log1p(rhs);
	// 	}
	// };
	// template<typename ValueType>
	// struct InverseScanOperation
	// {
	// 	constexpr ValueType operator()(ValueType lhs, ValueType rhs)
	// 	{
	// 		return std::expm1(lhs-rhs);
	// 	}
	// };

	// class StdInclusiveScanner
	// {
	// public:
	template<typename IIterator, typename ISentinel, typename OIterator>
	void scan(IIterator iBeg, ISentinel iEnd, OIterator oBeg)
	{
		using ValueType = typename std::iterator_traits<IIterator>::value_type;
		;
	}
	// };

	// class StdParUnseqInclusiveScanner
	// {
	// public:
	// 	template<typename IBeginIterator, typename IEndIterator, typename OBeginIterator>
	// 	void scan(IBeginIterator iBeg, IEndIterator iEnd, OBeginIterator oBeg) const
	// 	{
	// 		std::inclusive_scan(std::execution::par_unseq, iBeg, iEnd, oBeg);
	// 	}
	// };
} // namespace ex
