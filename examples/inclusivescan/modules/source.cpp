/**
 * @file source.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-02-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "source.h"

#include <algorithm>
#include <cassert>

namespace ex
{
		Source::Source(std::size_t maxSize):
			m_offsetGenerator(0, maxSize-1),
			m_data((2*maxSize)-1)
		{
			assert(maxSize != 0);
			assert(maxSize <= 2<<20);

			UniformGenerator<ValueType> dataGenerator;
			std::generate_n(m_data.begin(), maxSize, dataGenerator);
			std::copy_n(m_data.cbegin(), maxSize-1, m_data.begin()+maxSize);
		}
		
		std::vector<Source::ValueType> Source::get(std::size_t size)
		{
			assert(size <= (m_data.size()+1)/2);
			
			const auto begin = m_data.cbegin()+offset();
			return {begin, begin+size};
		}
		std::size_t Source::offset()
		{
			return m_offsetGenerator();
		}
} // namespace ex
