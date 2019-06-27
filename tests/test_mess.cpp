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

#include <algorithm>
#include <fstream>
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

struct TestChannelByValue {};
struct TestChannelByRef {};
// struct TestServiceByValue {};
// struct TestServiceByRef {};
// struct TestSingleProviderService {};

struct TestEndpoint1;
struct TestEndpoint2;

namespace mess
{
template<> struct Channel<TestChannelByRef>: Subscribers<TestEndpoint1, TestEndpoint2> {};
template<> struct Channel<TestChannelByValue>: Subscribers<TestEndpoint1, TestEndpoint2> {};
// template<> struct mess::Channel<TestServiceByRef>: mess::Subscribers<TestEndpoint1, TestEndpoint2> {};
// template<> struct mess::Channel<TestServiceByValue>: mess::Subscribers<TestEndpoint1, TestEndpoint2> {};
// template<> struct mess::Channel<TestSingleProviderService>: mess::Subscribers<TestEndpoint2> {};
} // namespace mess

// constexpr mess::Channel<TestServiceByRef>::ReturnType testEndpoint1TestServiceByRefReturn = 42;
// constexpr mess::Channel<TestServiceByValue>::ReturnType testEndpoint1TestServiceByValueReturn = 43.f;
// constexpr mess::Channel<TestServiceByRef>::ReturnType testEndpoint2TestServiceByRefReturn = 44;
// constexpr mess::Channel<TestServiceByValue>::ReturnType testEndpoint2TestServiceByValueReturn = 45.f;
// constexpr mess::Channel<TestSingleProviderService>::ReturnType testEndpoint2TestSingleProviderServiceReturn = 46;

struct TestEndpoint1
{
	using Component = TestEndpoint1;

	template<typename Broker>
	static void onPublish(TestChannelByRef, const Broker& broker, Component& core, const TestMessage& message) {}
	template<typename Broker>
	static void onPublish(TestChannelByValue, const Broker& broker, Component& core, TestMessage message) {}
	// template<typename Broker>
	// static mess::Channel<TestServiceByRef>::ReturnType onCall(TestServiceByRef, const Broker& broker, Component& core, const TestMessage& message)
	// {
	// 	return testEndpoint1TestServiceByRefReturn;
	// }
	// template<typename Broker>
	// static mess::Channel<TestServiceByValue>::ReturnType onCall(TestServiceByValue, const Broker& broker, Component& core, TestMessage message)
	// {
	// 	return testEndpoint1TestServiceByValueReturn;
	// }
};
struct TestEndpoint2: TestEndpoint1
{
// 	// template<typename Broker>
// 	// static mess::Channel<TestServiceByRef>::ReturnType onCall(TestServiceByRef, const Broker& broker, Component& core, const TestMessage& message)
// 	// {
// 	// 	return testEndpoint2TestServiceByRefReturn;
// 	// }
// 	// template<typename Broker>
// 	// static mess::Channel<TestServiceByValue>::ReturnType onCall(TestServiceByValue, const Broker& broker, Component& core, TestMessage message)
// 	// {
// 	// 	return testEndpoint2TestServiceByValueReturn;
// 	// }
// 	// template<typename Broker>
// 	// static mess::Channel<TestSingleProviderService>::ReturnType onCall(TestSingleProviderService, const Broker& broker, Component& core, const TestMessage& message)
// 	// {
// 	// 	return testEndpoint2TestSingleProviderServiceReturn;
// 	// }
};

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

TEST(HelloWorldTest, WithOrWithoutUnknown) {
	std::filebuf fb;
	fb.open("../helloworld", std::ios::in|std::ios::binary);
	const auto sizeWithoutUnknowns = fb.in_avail();
	std::vector<char> withoutUnknowns(sizeWithoutUnknowns);
	fb.sgetn(withoutUnknowns.data(), sizeWithoutUnknowns);
	fb.close();
	fb.open("../helloworldunknown", std::ios::in|std::ios::binary);
	const auto sizeWithUnknowns = fb.in_avail();
	std::vector<char> withUnknowns(sizeWithUnknowns);
	fb.sgetn(withUnknowns.data(), sizeWithUnknowns);

	EXPECT_EQ(sizeWithoutUnknowns, sizeWithUnknowns);
	EXPECT_TRUE(std::equal(withoutUnknowns.cbegin(), withoutUnknowns.cend(), withUnknowns.cbegin()));
}

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

	TestEndpoint1 component1;
	TestEndpoint2 component2;

	mess::Broker<TestEndpoint1, TestEndpoint2> broker{component1, component2};
	
	TestMessage message;
};

TEST_F(MessTest, ConstRefPublishSubByRef) {
	broker.publish<TestChannelByRef>(const_cast<const TestMessage&>(message));
	EXPECT_EQ(TestMessage::copyConstructed, 0);
	EXPECT_EQ(TestMessage::moveConstructed, 0);
}
TEST_F(MessTest, RefPublishSubByRef) {
	broker.publish<TestChannelByRef>(message);
	EXPECT_EQ(TestMessage::copyConstructed, 0);
	EXPECT_EQ(TestMessage::moveConstructed, 0);
}
TEST_F(MessTest, MovePublishSubByRef) {
	broker.publish<TestChannelByRef>(std::move(message));
	EXPECT_EQ(TestMessage::copyConstructed, 0);
	EXPECT_EQ(TestMessage::moveConstructed, 0);
}
TEST_F(MessTest, ConstRefPublishSubByValue) {
	broker.publish<TestChannelByValue>(const_cast<const TestMessage&>(message));
	EXPECT_EQ(TestMessage::copyConstructed, 2);
	EXPECT_EQ(TestMessage::moveConstructed, 0);
}
TEST_F(MessTest, RefPublishSubByValue) {
	broker.publish<TestChannelByValue>(message);
	EXPECT_EQ(TestMessage::copyConstructed, 2);
	EXPECT_EQ(TestMessage::moveConstructed, 0);
}
TEST_F(MessTest, MovePublishSubByValue) {
	broker.publish<TestChannelByValue>(std::move(message));
	EXPECT_EQ(TestMessage::copyConstructed, 1);
	EXPECT_EQ(TestMessage::moveConstructed, 1);
}

// // TEST_F(MessTest, ConstRefCallProvideByRef) {
// // 	auto response = broker.call<TestServiceByRef>(const_cast<const TestMessage&>(message));
// // 	EXPECT_EQ(TestMessage::copyConstructed, 0);
// // 	EXPECT_EQ(TestMessage::moveConstructed, 0);

// // 	EXPECT_EQ(std::get<0>(response), testEndpoint2TestServiceByRefReturn);
// // 	EXPECT_EQ(std::get<1>(response), testEndpoint1TestServiceByRefReturn);
// // }
// // TEST_F(MessTest, SingleProviderService) {
// // 	auto response = broker.call<TestSingleProviderService>(const_cast<const TestMessage&>(message));
// // 	EXPECT_EQ(TestMessage::copyConstructed, 0);
// // 	EXPECT_EQ(TestMessage::moveConstructed, 0);

// // 	EXPECT_EQ(response, testEndpoint2TestSingleProviderServiceReturn);
// // }