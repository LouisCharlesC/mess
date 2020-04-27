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

#include <mess/mess.h>

#include <cassert>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <vector>

struct Source:
	mess::IsALocalVariable
{};
struct DataSize:
	mess::IsALocalVariable
{};
struct OriginalData:
	mess::IsTheResultOfCalling<&ex::Source::get>,
	mess::OnInstance<Source>,
	mess::WithArgument<DataSize>
{};
struct ScannedData:
	mess::IsALocalVariable
{};
struct DoScan:
	mess::IsTheResultOfALocalLambda,
	mess::WithArguments<OriginalData, ScannedData>
{};
struct DoCheck:
	mess::IsTheResultOfALocalLambda,
	mess::WithArguments<ScannedData, OriginalData>
		// mess::WhenAll<DoScan>
{};

int main(int, char const *[])
{
	auto doScan = 
		[]
		(const auto& original, auto& scanned)
		{
			std::partial_sum(original.begin(), original.end(), scanned.begin());
		};
	auto doCheck =
		[]
		(const auto& scanned, const auto& original)
		{
			return ex::check(scanned.cbegin(), scanned.cend(), original.begin());
		};

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
		const auto original = mess::pull<OriginalData>(mess::push<Source>(source), mess::push<DataSize>(size));
		
		// Perform and time computation
		const auto beforeScan = std::chrono::high_resolution_clock::now();
		mess::pull<DoScan>(mess::push<DoScan>(doScan), mess::push<OriginalData>(original), mess::push<ScannedData>(scanned));
		scanTime += std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now()-beforeScan)/NumberOfIterationsToRun;

		// Perform dependent computations
		check &= mess::pull<DoCheck>(mess::push<DoCheck>(doCheck), mess::push<OriginalData>(original), mess::push<ScannedData>(scanned));
		assert(check);
	}
	const std::chrono::duration<double, std::milli> totalTime = (std::chrono::high_resolution_clock::now()-beforeAll) / NumberOfIterationsToRun;

	std::cout << "Scan was correct: " << std::boolalpha << check << std::endl << std::endl;

	std::cout << "Mean scan time: " << scanTime.count() << " ms" << std::endl;
	std::cout << "Mean total time: " << totalTime.count() << " ms" << std::endl;

	return 0;
}
