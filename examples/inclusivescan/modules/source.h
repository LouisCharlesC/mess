/**
 * @file datagenerator.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-11-13
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <cstddef>
#include <limits>
#include <random>
#include <vector>

namespace ex
{
	template<typename ValueType>
	struct UniformDistribution
	{
		using type = std::uniform_int_distribution<ValueType>;
	};
	template<>
	struct UniformDistribution<float>
	{
		using type = std::uniform_real_distribution<float>;
	};
	template<>
	struct UniformDistribution<double>
	{
		using type = std::uniform_real_distribution<double>;
	};

	class Source
	{
		template<typename ValueType>
		class UniformGenerator
		{
		public:
			UniformGenerator(ValueType min=std::numeric_limits<ValueType>::min(), ValueType max=std::numeric_limits<ValueType>::max()):
				m_uniformDistribution(min, max)
			{}

			ValueType operator()()
			{
				return m_uniformDistribution(m_randomEngine);
			}

		private:
			std::default_random_engine m_randomEngine;
			typename UniformDistribution<ValueType>::type m_uniformDistribution;
		};

	public:
		using ValueType = int;

		Source(std::size_t maxSize);
		
		std::vector<ValueType> get(std::size_t size);

	private:
		std::size_t offset();

		std::vector<ValueType> m_data;
		UniformGenerator<std::size_t> m_offsetGenerator;
	};
} // namespace ex
