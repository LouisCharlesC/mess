/**
 * @file test_mess.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-05-22
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <mess/mess.h>

#include <doctest/doctest.h>

#include <algorithm>
#include <fstream>
#include <vector>

TEST_CASE("Helloworld compiles to same binary with or without mess")
{
	std::filebuf fb;
	fb.open("../helloworld", std::ios::in|std::ios::binary);
	REQUIRE(fb.is_open());
	const auto sizeWithMess = fb.in_avail();
	std::vector<char> withMess(sizeWithMess);
	fb.sgetn(withMess.data(), sizeWithMess);
	fb.close();
	fb.open("../helloworldwomess", std::ios::in|std::ios::binary);
	REQUIRE(fb.is_open());
	const auto sizeWithoutMess = fb.in_avail();
	std::vector<char> withoutMess(sizeWithoutMess);
	fb.sgetn(withoutMess.data(), sizeWithoutMess);

	REQUIRE_EQ(sizeWithMess, sizeWithoutMess);
	CHECK(std::equal(withMess.cbegin(), withMess.cend(), withoutMess.cbegin()));
}
