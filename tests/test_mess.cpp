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

#include "mess/mess.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <algorithm>
#include <fstream>
#include <vector>

struct TestMessage
{
	TestMessage() = default;
	TestMessage(const TestMessage& other)
	{
		++other.copyConstructed;
	}
	TestMessage(TestMessage&& other)
	{
		++other.moveConstructed;
	}

	mutable int copyConstructed = 0;
	int moveConstructed = 0;
};

struct TestTopicByValue {};
struct TestTopicByRef {};

struct TestComponent1
{
	using Core = TestComponent1;

	template<typename Broker>
	static void onPublish(TestTopicByRef, Broker& broker, Core& core, const TestMessage& message) {}
	template<typename Broker>
	static void onPublish(TestTopicByValue, Broker& broker, Core& core, TestMessage message) {}

};
struct TestComponent2: TestComponent1 {};

template<> struct mess::Topic<TestTopicByValue>: mess::Subscribe<TestComponent1, TestComponent2> {};
template<> struct mess::Topic<TestTopicByRef>: mess::Subscribe<TestComponent1, TestComponent2> {};

TEST(HelloWorldTest, WithOrWithoutMess) {
	std::filebuf fb;
	fb.open("../helloworld", std::ios::in|std::ios::binary);
	const auto sizeWithMess = fb.in_avail();
	std::vector<char> withMess(sizeWithMess);
	fb.sgetn(withMess.data(), sizeWithMess);
	fb.close();
	fb.open("../helloworldwomess", std::ios::in|std::ios::binary);
	const auto sizeWithoutMess = fb.in_avail();
	std::vector<char> withoutMess(sizeWithoutMess);
	fb.sgetn(withoutMess.data(), sizeWithoutMess);

	EXPECT_EQ(sizeWithMess, sizeWithoutMess);
	EXPECT_TRUE(std::equal(withMess.cbegin(), withMess.cend(), withoutMess.cbegin()));
}

struct MessTest : public testing::Test
{
	TestComponent1 component1;
	TestComponent2 component2;

	mess::Broker<TestComponent1, TestComponent2> broker{component1, component2};
	
	TestMessage message;
};

TEST_F(MessTest, ConstRefPublishSubByRef) {
	broker.publish<TestTopicByRef>(const_cast<const TestMessage&>(message));
	EXPECT_EQ(message.copyConstructed, 0);
	EXPECT_EQ(message.moveConstructed, 0);
}
TEST_F(MessTest, RefPublishSubByRef) {
	broker.publish<TestTopicByRef>(message);
	EXPECT_EQ(message.copyConstructed, 0);
	EXPECT_EQ(message.moveConstructed, 0);
}
TEST_F(MessTest, MovePublishSubByRef) {
	broker.publish<TestTopicByRef>(std::move(message));
	EXPECT_EQ(message.copyConstructed, 0);
	EXPECT_EQ(message.moveConstructed, 0);
}
TEST_F(MessTest, ConstRefPublishSubByValue) {
	broker.publish<TestTopicByValue>(const_cast<const TestMessage&>(message));
	EXPECT_EQ(message.copyConstructed, 2);
	EXPECT_EQ(message.moveConstructed, 0);
}
TEST_F(MessTest, RefPublishSubByValue) {
	broker.publish<TestTopicByValue>(message);
	EXPECT_EQ(message.copyConstructed, 2);
	EXPECT_EQ(message.moveConstructed, 0);
}
TEST_F(MessTest, MovePublishSubByValue) {
	broker.publish<TestTopicByValue>(std::move(message));
	EXPECT_EQ(message.copyConstructed, 2);
	EXPECT_EQ(message.moveConstructed, 0);
}

// TEST(TestMess, ConstRefPublish) {
// 	TestComponent1 component1;
// 	TestComponent2 component2;
// 	mess::Broker<TestComponent1, TestComponent2> broker(component1, component2);
	
// 	const TestMessage message;
// 	broker.publish<TestTopicByRef>(message);
// 	EXPECT_EQ(message.copyConstructed, 0);
// 	EXPECT_EQ(message.moveConstructed, 0);
	
// 	broker.publish<TestTopicByValue>(message);
// 	EXPECT_EQ(message.copyConstructed, 2);
// 	EXPECT_EQ(message.moveConstructed, 0);
// }

// TEST(TestMess, MovePublishSubByRef) {
// 	broker.publish<TestTopicByRef>(std::move(message));
// 	EXPECT_EQ(message.copyConstructed, 0);
// 	EXPECT_EQ(message.moveConstructed, 0);

// 	broker.publish<TestTopicByValue>(std::move(message));
// 	EXPECT_EQ(message.copyConstructed, 2);
// 	EXPECT_EQ(message.moveConstructed, 0);
// }

// TEST(TestMess, MovePublishSubByValue) {
// 	TestComponent1 component1;
// 	TestComponent2 component2;
// 	mess::Broker<TestComponent1, TestComponent2> broker(component1, component2);
	
// 	TestMessage message;
// }