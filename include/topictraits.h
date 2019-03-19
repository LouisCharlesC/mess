/*
 * topictraits.h
 *
 *  Created on: Nov. 11, 2018
 *      Author: lcc
 */

#pragma once

#include <functional>
#include <type_traits>

namespace mess {
	struct PullDisabled_t {};
	static constexpr PullDisabled_t PullDisabled;
	struct PullEnabled_t {};
	static constexpr PullEnabled_t PullEnabled;
	template<typename TopicGroup, TopicGroup Topic>
	struct Message;
	template<typename TopicGroup, TopicGroup Topic>
	struct PullCapability;
	template<typename TopicGroup, TopicGroup Topic>
	struct Callback;

	/**
	 * @brief Utility type alias to get a topic's message type.
	 */
	template<typename TopicGroup, TopicGroup Topic>
	using MessageType = typename Message<TopicGroup, Topic>::type;
	/**
	 * @brief Utility function to get a topic's pull capability.
	 */
	template<typename TopicGroup, TopicGroup Topic>
	constexpr bool IsPullEnabled()
	{
		return std::is_base_of<PullEnabled_t, PullCapability<TopicGroup, Topic>>::value;
	}
	/**
	 * @brief Utility type alias to get a topic's callback type.
	 */
	template<typename TopicGroup, TopicGroup Topic>
	using CallbackType = typename Callback<TopicGroup, Topic>::type;

	/**
	 * @brief Specialize this trait and define a type alias named type to
	 * specify what is the type of TopicGroup::Topic's messages.
	 * 
	 * If not specialized, the topic has no message and subscribers can
	 * be notify'ed of events.
	 */
	template<typename TopicGroup, TopicGroup Topic>
	struct Message
	{
		using type = void;
	};
	/**
	 * @brief Specialize this and have the specialization inherit from
	 * PullEnabled_t to allow pull capability.
	 */
	template<typename TopicGroup, TopicGroup Topic>
	struct PullCapability: PullDisabled_t
	{};
	/**
	 * @brief Specialize this trait and define a type alias named type to
	 * specify what is the type of TopicGroup::Topic's callbacks.
	 */
	template<typename TopicGroup, TopicGroup Topic>
	struct Callback
	{
		using type = std::function<void(const MessageType<TopicGroup, Topic>&)>;
	};
	/**
	 * @brief Specialize this function to specify the number of topics in
	 * TopicGroup.
	 */
	template<typename TopicGroup>
	constexpr std::size_t Count()
	{
		return static_cast<std::size_t>(TopicGroup::Count);
	}
}  // namespace mess
