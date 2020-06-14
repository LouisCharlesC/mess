/**
 * @file main.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-02-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <checker.h>
#include <scanner.h>
#include <source.h>

#include <cassert>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <vector>

int main(int, char const *[])
{
	constexpr std::size_t size = 1<<17;
	constexpr std::size_t maxSize = size;
	static_assert(size<=maxSize, "");

	static ex::Source source(maxSize);
	static std::vector<int> scanned(maxSize);

	bool check = true;
	constexpr std::size_t NumberOfIterationsToRun = 1000;
	std::chrono::duration<double, std::milli> scanTime = std::chrono::duration<double, std::milli>::zero();
	const auto beforeAll = std::chrono::high_resolution_clock::now();
	for (std::size_t i = 0; i < NumberOfIterationsToRun; ++i)
	{
		// Get some data
		const auto original = source.get(size);
		
		// Perform and time computation
		const auto beforeScan = std::chrono::high_resolution_clock::now();
		std::partial_sum(original.begin(), original.end(), scanned.begin());
		scanTime += std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now()-beforeScan)/NumberOfIterationsToRun;

		// Perform dependent computations
		check &= ex::check(scanned.cbegin(), scanned.cend(), original.begin());
		assert(check);
	}
	const std::chrono::duration<double, std::milli> totalTime = (std::chrono::high_resolution_clock::now()-beforeAll) / NumberOfIterationsToRun;

	std::cout << "Scan was correct: " << std::boolalpha << check << std::endl << std::endl;

	std::cout << "Mean scan time: " << scanTime.count() << " ms" << std::endl;
	std::cout << "Mean total time: " << totalTime.count() << " ms" << std::endl;

	return 0;
}
