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
#include <tuple>
#include <vector>

struct TestMessage
{
	TestMessage() = default;
	TestMessage(const TestMessage& other)
	{
		++TestMessage::copyConstructed;
	}
	TestMessage(TestMessage&& other)
	{
		++TestMessage::moveConstructed;
	}

	static int copyConstructed;
	static int moveConstructed;
};
int TestMessage::copyConstructed;
int TestMessage::moveConstructed;

struct TestTopicByValue {};
struct TestTopicByRef {};
struct TestServiceByValue {};
struct TestServiceByRef {};
struct TestSingleProviderService {};

struct TestComponent1;
struct TestComponent2;

template<> struct mess::Topic<TestTopicByRef>: mess::Subscribers<TestComponent1, TestComponent2> {};
template<> struct mess::Topic<TestTopicByValue>: mess::Subscribers<TestComponent1, TestComponent2> {};
template<> struct mess::Topic<TestServiceByRef>: mess::Return<int>, mess::Providers<TestComponent1, TestComponent2> {};
template<> struct mess::Topic<TestServiceByValue>: mess::Return<float>, mess::Providers<TestComponent1, TestComponent2> {};
template<> struct mess::Topic<TestSingleProviderService>: mess::Return<char>, mess::Providers<TestComponent2> {};

constexpr mess::Topic<TestServiceByRef>::ReturnType testComponent1TestServiceByRefReturn = 42;
constexpr mess::Topic<TestServiceByValue>::ReturnType testComponent1TestServiceByValueReturn = 43.f;
constexpr mess::Topic<TestServiceByRef>::ReturnType testComponent2TestServiceByRefReturn = 44;
constexpr mess::Topic<TestServiceByValue>::ReturnType testComponent2TestServiceByValueReturn = 45.f;
constexpr mess::Topic<TestSingleProviderService>::ReturnType testComponent2TestSingleProviderServiceReturn = 46;

struct TestComponent1
{
	using Core = TestComponent1;

	template<typename Broker>
	static void onPublish(TestTopicByRef, const Broker& broker, Core& core, const TestMessage& message) {}
	template<typename Broker>
	static void onPublish(TestTopicByValue, const Broker& broker, Core& core, TestMessage message) {}
	template<typename Broker>
	static mess::Topic<TestServiceByRef>::ReturnType onCall(TestServiceByRef, const Broker& broker, Core& core, const TestMessage& message)
	{
		return testComponent1TestServiceByRefReturn;
	}
	template<typename Broker>
	static mess::Topic<TestServiceByValue>::ReturnType onCall(TestServiceByValue, const Broker& broker, Core& core, TestMessage message)
	{
		return testComponent1TestServiceByValueReturn;
	}

};
struct TestComponent2: TestComponent1
{
	template<typename Broker>
	static mess::Topic<TestServiceByRef>::ReturnType onCall(TestServiceByRef, const Broker& broker, Core& core, const TestMessage& message)
	{
		return testComponent2TestServiceByRefReturn;
	}
	template<typename Broker>
	static mess::Topic<TestServiceByValue>::ReturnType onCall(TestServiceByValue, const Broker& broker, Core& core, TestMessage message)
	{
		return testComponent2TestServiceByValueReturn;
	}
	template<typename Broker>
	static mess::Topic<TestSingleProviderService>::ReturnType onCall(TestSingleProviderService, const Broker& broker, Core& core, const TestMessage& message)
	{
		return testComponent2TestSingleProviderServiceReturn;
	}
};

#ifdef NDEBUG
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
#endif

struct MessTest : public testing::Test
{
	MessTest()
	{
		MessTest::ResetCounters();
	}

	static void ResetCounters()
	{
		TestMessage::copyConstructed = 0;
		TestMessage::moveConstructed = 0;
	}

	TestComponent1 component1;
	TestComponent2 component2;

	const mess::Broker<TestComponent1, TestComponent2> broker{component1, component2};
	
	TestMessage message;
};

TEST_F(MessTest, ConstRefPublishSubByRef) {
	broker.publish<TestTopicByRef>(const_cast<const TestMessage&>(message));
	EXPECT_EQ(TestMessage::copyConstructed, 0);
	EXPECT_EQ(TestMessage::moveConstructed, 0);
}
TEST_F(MessTest, RefPublishSubByRef) {
	broker.publish<TestTopicByRef>(message);
	EXPECT_EQ(TestMessage::copyConstructed, 0);
	EXPECT_EQ(TestMessage::moveConstructed, 0);
}
TEST_F(MessTest, MovePublishSubByRef) {
	broker.publish<TestTopicByRef>(std::move(message));
	EXPECT_EQ(TestMessage::copyConstructed, 0);
	EXPECT_EQ(TestMessage::moveConstructed, 0);
}
TEST_F(MessTest, ConstRefPublishSubByValue) {
	broker.publish<TestTopicByValue>(const_cast<const TestMessage&>(message));
	EXPECT_EQ(TestMessage::copyConstructed, 2);
	EXPECT_EQ(TestMessage::moveConstructed, 0);
}
TEST_F(MessTest, RefPublishSubByValue) {
	broker.publish<TestTopicByValue>(message);
	EXPECT_EQ(TestMessage::copyConstructed, 2);
	EXPECT_EQ(TestMessage::moveConstructed, 0);
}
TEST_F(MessTest, MovePublishSubByValue) {
	broker.publish<TestTopicByValue>(std::move(message));
	EXPECT_EQ(TestMessage::copyConstructed, 1);
	EXPECT_EQ(TestMessage::moveConstructed, 1);
}

TEST_F(MessTest, ConstRefCallProvideByRef) {
	auto response = broker.call<TestServiceByRef>(const_cast<const TestMessage&>(message));
	EXPECT_EQ(TestMessage::copyConstructed, 0);
	EXPECT_EQ(TestMessage::moveConstructed, 0);

	EXPECT_EQ(std::get<0>(response), testComponent2TestServiceByRefReturn);
	EXPECT_EQ(std::get<1>(response), testComponent1TestServiceByRefReturn);
}
TEST_F(MessTest, SingleProviderService) {
	auto response = broker.call<TestSingleProviderService>(const_cast<const TestMessage&>(message));
	EXPECT_EQ(TestMessage::copyConstructed, 0);
	EXPECT_EQ(TestMessage::moveConstructed, 0);

	EXPECT_EQ(response, testComponent2TestSingleProviderServiceReturn);
}