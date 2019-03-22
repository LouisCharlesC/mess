/**
 * @file subscribemacros.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-20
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#define MESS_SUBSCRIBE_TEMPLATE(Subscriber, Topic) \
namespace mess \
{ \
	template<> \
	void onPublish<Topic, Subscriber>(const Topic::Message& message, Subscriber& subscriber) \
	{ \
		subscriber.onPublish<Topic>(message); \
	} \
}

#define MESS_SUBSCRIBE_TAG(Subscriber, Topic) \
namespace mess \
{ \
	template<> \
	void onPublish<Topic, Subscriber>(const Topic::Message& message, Subscriber& subscriber) \
	{ \
		subscriber.onPublish(message, Topic()); \
	} \
}
