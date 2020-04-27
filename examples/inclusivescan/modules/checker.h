/**
 * @file checker.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-11-13
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>

namespace ex
{
	template<typename ScannedIterator, typename ScannedSentinel, typename OriginalIterator>
	bool check(ScannedIterator scannedBeg, ScannedSentinel scannedEnd, OriginalIterator originalBeg)
	{
		using ValueType = typename std::iterator_traits<ScannedIterator>::value_type;
		
		const auto size = std::distance(scannedBeg, scannedEnd);
		assert(size >= 0);
		const auto unscanned = std::make_unique<ValueType[]>(static_cast<std::size_t>(size));

		unscanned[0] = *scannedBeg;
		std::transform(std::next(scannedBeg), scannedEnd, scannedBeg, unscanned.get()+1, std::minus<ValueType>());

		const auto check = std::equal(unscanned.get(), unscanned.get()+size, originalBeg);
		return check;
	}
} // namespace ex

// #include <execution>
// std::transform(std::execution::par_unseq, std::next(scannedBeg), scannedEnd, scannedBeg, std::next(unscanned.begin()), std::minus());
// const auto check = std::equal(std::execution::par_unseq, originalBeg, originalEnd, unscanned.cbegin());