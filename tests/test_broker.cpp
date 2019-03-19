/**
 * @file test_broker.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "broker.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

/// Test for scoped enums
enum class ScopedTopics: std::size_t
{
	FirstScopedTopic,
	SecondScopedTopic
};
/// Also test for unscoped enums
enum Topics: std::size_t
{
	FirstTopic = 0,
	SecondTopic,
	Count
};
/// Test for specialized callbacks
static auto callbackLambda = [](const int& i){std::cout << "Got message " << i << " on topic ScopedTopics::FirstScopedTopic" << std::endl;};
void callbackFreeFunction(const float& j) {std::cout << "Got message " << j << " on topic ScopedTopics::SecondScopedTopic" << std::endl;}

namespace mess
{
	/// Specializations for ScopedTopics::FirstScopedTopic
	template<>
	struct Message<ScopedTopics, ScopedTopics::FirstScopedTopic>
	{
		using type = int;
	};
	template<>
	struct PullCapability<ScopedTopics, ScopedTopics::FirstScopedTopic>: PullEnabled_t {};
	template<>
	struct Callback<ScopedTopics, ScopedTopics::FirstScopedTopic>
	{
		using type = decltype(callbackLambda);
	};
	template<>
	constexpr std::size_t Count<ScopedTopics>()
	{
		return 2;
	}
	/// Specialization for ScopedTopics::FirstScopedTopic
	template<>
	struct Message<ScopedTopics, ScopedTopics::SecondScopedTopic>
	{
		using type = float;
	};

	/// Specializations for unscoped Topics
	template<>
	struct Message<Topics, FirstTopic>
	{
		using type = float;
	};
	template<>
	struct Message<Topics, SecondTopic>
	{
		using type = float;
	};

}

TEST(EnumPack, bla) {
	constexpr int i = 42;
	constexpr float j = 42.f;

	mess::MasterBroker<Topics, ScopedTopics> mbroker;
	auto broker = mbroker.make_broker<Topics, ScopedTopics>();
	{
		auto firstGroupBroker = mbroker.make_broker<Topics>();
		auto lastGroupBroker = mbroker.make_broker<ScopedTopics>();
		auto reverseBroker = mbroker.make_broker<ScopedTopics, Topics>();
	}

	broker.subscribe<ScopedTopics, ScopedTopics::FirstScopedTopic>(callbackLambda);
	broker.subscribe<ScopedTopics, ScopedTopics::SecondScopedTopic>(callbackFreeFunction);

	broker.push<ScopedTopics, ScopedTopics::FirstScopedTopic>(i);
	const int iPull = broker.pull<ScopedTopics, ScopedTopics::FirstScopedTopic>().get();
	EXPECT_EQ(i, iPull);
	broker.push<ScopedTopics, ScopedTopics::SecondScopedTopic>(j);
}