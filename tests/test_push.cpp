/**
 * @file test_push.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-04-27
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "utils/operationcounter.h"

#include <mess/mess.h>

#include <doctest/doctest.h>

// TEST_CASE("")
// {
// 	auto pi = mess::push<I>(std::move(i));
// 	auto gpi = pi.get<I>();
// 	static_assert(std::is_same_v<decltype(mess::push<I>(std::move(i)).get<I>()), int&&>);
// 	CHECK_EQ(gpi, i);
// }

// // test push different & and &&.
struct TestCounter;
// static_assert(std::is_same_v<decltype(mess::push<TestCounter>(static_cast<const OperationCounter&>(counter)).get<TestCounter>()), const OperationCounter&>);
// static_assert(std::is_same_v<decltype(mess::push<TestCounter>(std::move(counter)).get<TestCounter>()), OperationCounter&&>);


// TEST_CASE("Pull")
// {
// 	struct Allo;
// 	struct Allo2;
// 	const int allo = 42;
// 	const mess::NamedValue<Allo, int> pallo = mess::push<Allo>(allo);

// }

TEST_CASE("Push")
{
	OperationCounter counter;
	
	SUBCASE("Value")
	{
	SUBCASE("Push")
	{
		auto pushed = mess::push<TestCounter>(counter);
		static_assert(std::is_same_v<decltype(pushed.value), OperationCounter>);

		CHECK(pushed.value.checkCounts(1, 0, 0, 0, false));
	}
	SUBCASE("Push ref")
	{
		auto pushed = mess::push<TestCounter>(std::ref(counter));
		static_assert(std::is_same_v<decltype(pushed.value), OperationCounter&>);

		CHECK(pushed.value.checkCounts(0, 0, 0, 0, false));
	}
	SUBCASE("Push cref")
	{
		auto pushed = mess::push<TestCounter>(std::cref(counter));
		static_assert(std::is_same_v<decltype(pushed.value), const OperationCounter&>);

		CHECK(pushed.value.checkCounts(0, 0, 0, 0, false));
	}
	}
	
// 	SUBCASE("Get")
// 	{
// 		auto gotten = pushed.get<TestCounter>();
// 		CHECK(gotten.checkCounts(1, 0, 0, 0, false));
// 	}
// 	SUBCASE("Get decltype")
// 	{
// 		decltype(auto) gotten = pushed.get<TestCounter>();
// 		CHECK(gotten.checkCounts(0, 0, 0, 0, false));
// 	}
// 	SUBCASE("Get &")
// 	{
// 		auto& gotten = pushed.get<TestCounter>();
// 		CHECK(gotten.checkCounts(0, 0, 0, 0, false));
// 	}
// 	SUBCASE("Get &&")
// 	{
// 		auto&& gotten = pushed.get<TestCounter>();
// 		CHECK(gotten.checkCounts(0, 0, 0, 0, false));
// 	}
}

// TEST_CASE("Push const")
// {
// 	auto pi = mess::push<I>(std::move(i));
// 	auto gpi = pi.get<I>();
// 	static_assert(std::is_same_v<decltype(mess::push<I>(std::move(i)).get<I>()), int&&>);
// 	CHECK_EQ(gpi, i);
// }

// TEST_CASE("Push move")
// {
// 	auto pi = mess::push<I>(std::move(i));
// 	auto gpi = pi.get<I>();
// 	static_assert(std::is_same_v<decltype(mess::push<I>(std::move(i)).get<I>()), int&&>);
// 	CHECK_EQ(gpi, i);
// }