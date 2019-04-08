/**
 * @file messtraits.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-25
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

namespace mess
{
	// Base traits type
	template<typename TopicType>
	struct Topic;

	// Helper trait type aliases
	template<typename TopicType>
	using Return = typename Topic<TopicType>::Return;
	template<typename TopicType>
	using Provider = typename Topic<TopicType>::Provider;

	// Have your specialization of Topic inherit from this to create a Notification topic
	template<typename TopicType>
	struct Notification
	{
		template<typename Subscriber>
		static void onNotify(Subscriber&) {};
	};
	// Have your specialization of Topic inherit from this to create a Publication topic
	template<typename TopicType, typename... ArgTypes>
	struct Publication
	{
		template<typename Subscriber>
		static void onNotify(Subscriber&) {};
		template<typename Subscriber>
		static void onPublish(Subscriber&, ArgTypes... args) {};
	};
	// Have your specialization of Topic inherit from this to create a Service topic
	template<typename TopicType, typename ProviderType, typename ReturnType, typename... ArgTypes>
	struct Service
	{
		using Return = ReturnType;
		using Provider = ProviderType;

		static Return onCall(Provider&, ArgTypes... args);
	};
} // namespace mess