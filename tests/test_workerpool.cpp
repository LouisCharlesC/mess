/**
 * @file test_workerpool.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "workerpool.h"
#include "workpromise.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <chrono>
#include <stack>
#include <thread>

TEST(allo, blah)
{
	mess::WorkerPool<std::stack<mess::ExecutableWork>> wpool(2);
	auto submitter = wpool.getWorkSubmitter();
	submitter([](){std::cout << "Executed!" << std::endl;});


	std::this_thread::sleep_for(std::chrono::seconds(1));
}