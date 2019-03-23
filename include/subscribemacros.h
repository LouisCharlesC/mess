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

#define MESS_SUBSCRIBE(Subscriber, Topic) \
namespace mess \
{ \
	template<> \
	void onPublish<Topic, Subscriber>(const Topic::Message& message, Subscriber& subscriber) \
	{ \
		subscriber.onPublish(message, Topic()); \
	} \
}
#define MESS_SUBSCRIBE_NOTIFICATION(Subscriber, Topic) \
namespace mess \
{ \
	template<> \
	void onNotify<Topic, Subscriber>(Subscriber& subscriber) \
	{ \
		subscriber.onNotify(Topic()); \
	} \
}
