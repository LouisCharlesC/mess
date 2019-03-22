/**
 * @file test_staticbroker.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "brokertemplate.hpp"
#include "subscribemacros.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

class FirstSubscriber;
class SecondSubscriber;
enum class Subscribers
{
	FirstSubscriber,
	SecondSubscriber
};
using Broker = mess::BrokerTemplate<FirstSubscriber, SecondSubscriber>;

struct CamImageTopic
{
	using Message = int;
	static constexpr bool IsCacheEnabled = false;
};
struct MicSoundTopic
{
	using Message = float;
	static constexpr bool IsCacheEnabled = false;
};

class FirstSubscriber
{
public:
	void onPublish(const CamImageTopic::Message& message, CamImageTopic)
	{
		std::cout << "published to cam image topic, received by first subscriber" << std::endl;
	}
};
MESS_SUBSCRIBE_TAG(FirstSubscriber, CamImageTopic)

class SecondSubscriber
{

};

TEST(stttta, ticcc)
{
	FirstSubscriber first;
	SecondSubscriber second;

	Broker::subscriber<Subscribers, Subscribers::FirstSubscriber>(first);
	Broker::subscriber<Subscribers, Subscribers::SecondSubscriber>(second);

	CamImageTopic::Message image = 42;
	MicSoundTopic::Message sound = 24.f;

	Broker::publish<CamImageTopic>(image);
}