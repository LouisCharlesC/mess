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

#include "mess/mess.h"

#include <chrono>
#include <cstddef>
#include <iostream>

// Declare components
class DummySubscriber {};
// Alias for the Broker
using Broker = mess::BrokerTemplate<DummySubscriber>;

// Declare topics
struct TopicWithNoSubscriber;
struct TopicWithOneSubscriber;

namespace mess
{
	// Register topics
	template<> struct Topic<TopicWithNoSubscriber>: Publication<TopicWithNoSubscriber, int> {};
	template<> struct Topic<TopicWithOneSubscriber>: Publication<TopicWithOneSubscriber, int> {};

	// Define dummy callback for topic TopicWithOneSubscriber
	template<>
	template<>
	void Topic<TopicWithOneSubscriber>::Publication::onPublish(DummySubscriber&, int message)
	{
		std::cout << "Ignore this. " << message << std::endl;
	}
}

int loop()
{
	int x = 0;
	int k = 0;
	for (std::uint64_t i = 0; i < 1000000000; ++i)
	{
		x += k;
		++k;
	}
	return x;
}
// If Topic is TopicWithNoSubscriber or TopicWithOneSubscriber, loop then publish
template<typename Topic>
void loopThenPublish(Broker& broker)
{
	const auto x = loop();
	broker.publish<Topic>(x);
}
// If Topic is LoopThenDontPublish, just loop, do not publish (this is the baseline)
struct LoopThenDontPublish;
template<>
void loopThenPublish<LoopThenDontPublish>(Broker& broker)
{
	const auto x = loop();
}
template<typename Topic>
std::chrono::nanoseconds measure(Broker& broker)
{
	const auto beg = std::chrono::high_resolution_clock::now();
	loopThenPublish<Topic>(broker);
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-beg);
}

int main(int argc, char **argv)
{
	// Instantiate broker
	Broker broker;
	// Instantiate, and manually connect dummy component
	DummySubscriber dummy;
	broker.component(dummy);
	
	// Loop then do not publish, the whole thing should be optimized out
	{
		const auto duration_ns = measure<LoopThenDontPublish>(broker);
		std::cout << "Duration for no publish (baseline): " << duration_ns.count() << " ns (" << std::chrono::duration_cast<std::chrono::milliseconds>(duration_ns).count() << " ms)." << std::endl;
	}
	// Loop then publish to topic with no subscriber, the whole thing should also be optimized out
	{
		const auto duration_ns = measure<TopicWithNoSubscriber>(broker);
		std::cout << "Duration for publish with no subscriber: " << duration_ns.count() << " ns (" << std::chrono::duration_cast<std::chrono::milliseconds>(duration_ns).count() << " ms)." << std::endl;
	}
	// Loop then publish to topic with one subscriber, you pay for the loop
	{
		const auto duration_ns = measure<TopicWithOneSubscriber>(broker);
		std::cout << "Duration for publish with one subscriber: " << duration_ns.count() << " ns (" << std::chrono::duration_cast<std::chrono::milliseconds>(duration_ns).count() << " ms)." << std::endl;
	}

	// Rejoice
	return EXIT_SUCCESS;
}