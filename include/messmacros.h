/**
 * @file messmacros.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-20
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "brokertemplate.hpp"
#include "messtraits.h"

#include <utility>

#define MESS_ADVERTIZE_NOTIFICATION(topic_name) \
struct topic_name {}; \
namespace mess \
{ \
	template<> \
	struct TopicTraits<topic_name> \
	{ \
		static constexpr bool IsCacheEnabled = false; \
	}; \
}

#define MESS_ADVERTIZE_PUBLICATION(topic_name, message_type) \
struct topic_name {}; \
namespace mess \
{ \
	template<> \
	struct TopicTraits<topic_name> \
	{ \
		static constexpr bool IsCacheEnabled = false; \
		using Message = message_type; \
	}; \
}

#define MESS_ADVERTIZE_CACHE(topic_name, message_type, initial_value) \
struct topic_name {}; \
namespace mess \
{ \
	template<> \
	struct TopicTraits<topic_name> \
	{ \
		static constexpr bool IsCacheEnabled = true; \
		using Message = message_type; \
		static const Message<topic_name> InitialValue = initial_value; \
	}; \
	const Message<topic_name> TopicTraits<topic_name>::InitialValue; \
}

// #define MESS_ADVERTIZE_SINK(topic_name, message_type, sink_name) \
MESS_ADVERTIZE_PUBLICATION(topic_name, message_type) \
namespace mess \
	template<> \
	void onMove<topic_name, sink_name>(sink_name& sink, message_type&& message) \
	{ \
		sink.onMove(topic_name(), std::move(message)); \
	} \
}
// #define MESS_ADVERTIZE_SINK(topic_name, message_type, sink_name) \
MESS_ADVERTIZE_PUBLICATION(topic_name, message_type) \
namespace mess \
	void onMove(sink_name& sink, message_type&& message) \
	{ \
		sink.onMove(topic_name(), std::move(message)); \
	} \
}

// #define MESS_PROVIDE_SERVICE(topic_name, message_type, provider_name) \
MESS_ADVERTIZE_PUBLICATION(topic_name, message_type) \
namespace mess \
{ \
	template<> \
	void onCall<topic_name, provider_name>(provider_name& provider, message_type&& message) \
	{ \
		provider.oncall(topic_name(), std::move(message)); \
	} \
}
// #define MESS_PROVIDE_SERVICE(topic_name, message_type, provider_name) \
MESS_ADVERTIZE_PUBLICATION(topic_name, message_type) \
namespace mess \
{ \
	template<typename... Args> \
	void onCall(provider_name& provider, Args&&... args) \
	{ \
		provider.oncall(topic_name(), std::forward<Args>(args)...); \
	} \
}

#define MESS_SUBSCRIBE_NOTIFICATION(topic_name, subscriber_name) \
namespace mess \
{ \
	template<> void onNotify<topic_name, subscriber_name>(subscriber_name& subscriber); \
}

#define MESS_SUBSCRIBE_PUBLICATION(topic_name, subscriber_name) \
namespace mess \
{ \
	template<> void onPublish<topic_name, subscriber_name>(subscriber_name& subscriber, const Message<topic_name>& message); \
}

