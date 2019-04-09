/**
 * @file traits.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-25
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <type_traits>

namespace mess
{
	namespace trick
	{
		/// Clever template meta-programming to check if all elements of a
		/// variadic parameter pack fulfil a given condition. I wish I
		/// invented it.
		template<bool...> struct bool_pack;
		template<bool... bools> 
		using all_false = std::is_same<bool_pack<bools..., false>, bool_pack<false, bools...>>;
	}

	/**
	 * @brief Base trait class to define topics.
	 * 
	 * @tparam TopicType The topic.
	 */
	template<typename TopicType>
	struct Topic;

	// Helper trait type aliases
	template<typename TopicType>
	using Return = typename Topic<TopicType>::Return;
	template<typename TopicType>
	using Provider = typename Topic<TopicType>::Provider;

	/**
	 * @brief Specialize Topic for your topic and have it inherit from
	 * this class to define a Notification topic.
	 * 
	 * @tparam TopicType The topic.
	 */
	template<typename TopicType>
	struct Notification
	{
		template<typename Subscriber>
		static void onNotify(Subscriber&) {};
	};

	/**
	 * @brief Specialize Topic for your topic and have it inherit from
	 * this class to define a Publication topic.
	 * 
	 * Do not use rvalue references as arguments for a publication
	 * callback because several of these callbacks will be called 
	 * (one call per subscribing component) and this may cause a move
	 * operation to be called multiple times on the same variable. If you
	 * want to move something from a component to another, use a service.
	 * If you want to distribute something to multiple components and
	 * move it to the last component, publish to all components but one
	 * and then use a service from this last component to move the data.
	 * 
	 * @tparam TopicType The topic.
	 * @tparam ArgTypes The publication callbacks' arguments.
	 */
	template<typename TopicType, typename... ArgTypes>
	struct Publication
	{
		static_assert(trick::all_false<std::is_rvalue_reference<ArgTypes>::value...>::value, "Do not use rvalue references as publication arguments!");

		template<typename Subscriber>
		static void onNotify(Subscriber&) {};
		template<typename Subscriber>
		static void onPublish(Subscriber&, ArgTypes... args) {};
	};

	/**
	 * @brief Specialize Topic for your topic and have it inherit from
	 * this class to define a Service topic.
	 * 
	 * @tparam TopicType The topic.
	 * @tparam ProviderType The service provider.
	 * @tparam ReturnType The service callback's return type.
	 * @tparam ArgTypes The service callback's arguments.
	 */
	template<typename TopicType, typename ProviderType, typename ReturnType, typename... ArgTypes>
	struct Service
	{
		using Return = ReturnType;
		using Provider = ProviderType;

		static Return onCall(Provider&, ArgTypes... args);
	};
} // namespace mess