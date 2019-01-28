//============================================================================
// Name        : testresource.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "resource.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

class TestResource : public testing::Test {
public:
	mess::Resource<int> resource;
};

// get then get give two different resources
// get destroy then get gives the same resource
// push then get gives the pushed resource

TEST_F(TestResource, GetThenGet) {
	const auto res1 = resource.get();
	const auto res2 = resource.get();
	EXPECT_NE(&*res1, &*res2);
}

TEST_F(TestResource, GetDestroyThenGet) {
	const auto* ptr1 = &*resource.get();
	const auto res2 = resource.get();
	EXPECT_EQ(ptr1, &*res2);
}

TEST_F(TestResource, PushThenGet) {
	resource.push(42);
	EXPECT_EQ(*resource.get(), 42);
}