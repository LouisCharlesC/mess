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
#include <stats.h>

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

	constexpr std::size_t size = 1<<16;
	constexpr std::size_t maxSize = size;
	static_assert(size<=maxSize, "");

	static ex::Source source(maxSize);
	static std::vector<int> scanned(maxSize);

	ex::Stats scanTimeStats;
	ex::Stats totalTimeStats;

	bool check = true;
	constexpr std::size_t NumberOfIterationsToRun = 1000;
	for (std::size_t i = 0; i < NumberOfIterationsToRun; ++i)
	{
		const auto beforeAll = std::chrono::high_resolution_clock::now();

		// Get some data
		// const auto original = source.get(size);
		const auto original = mess::pull<OriginalData>(mess::push<Source>(source), mess::push<DataSize>(size));
		
		// Perform computation
		const auto beforeScan = std::chrono::high_resolution_clock::now();
		// std::partial_sum(original.begin, original.end, scanned.begin());
		mess::pull<DoScan>(mess::push<DoScan>(doScan), mess::push<OriginalData>(original), mess::push<ScannedData>(scanned));

		const auto afterScan = std::chrono::high_resolution_clock::now();

		// Perform dependent computations
		// check &= ex::check(scanned.cbegin(), scanned.cend(), original.begin);
		check &= mess::pull<DoCheck>(mess::push<DoCheck>(doCheck), mess::push<OriginalData>(original), mess::push<ScannedData>(scanned));
		assert(check);

		const auto afterAll = std::chrono::high_resolution_clock::now();
		totalTimeStats(afterAll-beforeAll);
		scanTimeStats(afterScan-beforeScan);
	}

	std::cout << "Scan was correct: " << std::boolalpha << check << std::endl << std::endl;

	std::cout <<
		"Min: " << scanTimeStats.min().count() << " ms,\n" <<
		"median: " << scanTimeStats.median().count() << " ms,\n" <<
		"max: " << scanTimeStats.max().count() << " ms,\n" <<
		"mean: " << scanTimeStats.mean().count() << " ms,\n" <<
		"and stddev: " << scanTimeStats.stddev().count() << " ms" <<
		" of scan time." << std::endl;
	std::cout <<
		"Min: " << totalTimeStats.min().count() << " ms,\n" <<
		"median: " << totalTimeStats.median().count() << " ms,\n" <<
		"max: " << totalTimeStats.max().count() << " ms,\n" <<
		"mean: " << totalTimeStats.mean().count() << " ms,\n" <<
		"and stddev: " << totalTimeStats.stddev().count() << " ms" <<
		" of total time." << std::endl;

	return 0;
}
