/**
 * @file tests.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-10-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <mess/mess.h>

#include <doctest/doctest.h>

TEST_CASE("")
{



	
	CHECK_EQ(mess::scope()(1, 2, 3, 4), 26);
}
