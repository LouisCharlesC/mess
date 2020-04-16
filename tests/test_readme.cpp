/**
 * @file test_readme.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-04-16
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <mess/mess.h>

#include <doctest/doctest.h>

struct IFilter
{
	virtual bool filter(bool b)
	{
		return !b;
	}
};
struct Filter: IFilter
{
	inline bool filter(bool b) override
	{
		return b;
	}
};
Filter filter;
bool param;

struct LowPassFilter:
	mess::IsPulledFrom<&filter>
{};
struct GoodLowPassParameter:
	mess::IsPulledFrom<&param>
{};
struct FilteredValue:
	mess::IsPulledFrom<&IFilter::filter>,
	mess::OnInstance<LowPassFilter>,
	mess::WithArgument<GoodLowPassParameter>
{};

TEST_CASE("readme example code")
{
	param = true;
	CHECK(::mess::pull<FilteredValue>());
	param = false;
	CHECK_FALSE(::mess::pull<FilteredValue>());
}