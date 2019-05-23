/**
 * @file mess.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <tuple>

// TODO: test for publish and call with different subscribers and providers patterns
			// add nested struct in components to facilitate explicit instantiation
			// show how to split into files and use expl inst to limit compilation time
			// allow returns for publications ?

namespace mess
{
	/**
	 * @brief Base trait class to define topics.
	 * 
	 * @tparam TopicType The topic.
	 */
	template<typename TopicType>
	struct Topic;

	template<typename... ComponentTypes>
	struct Subscribe
	{
		template<typename TopicType, typename BrokerType, typename... Args>
		static void publish(BrokerType& broker, Args&&... args)
		{
			// Sorry for this line of code, it just calls the onPublish member function from all subcribing components, forwarding args on each call
			(void)std::initializer_list<int>{((void)ComponentTypes::onPublish(TopicType(), broker, std::get<BrokerType::template ComponentIndex<ComponentTypes>::value>(broker.m_cores), std::forward<Args>(args)...),0)...};
		}
	};

	template<typename ComponentType>
	struct Provide
	{
		template<typename TopicType, typename BrokerType, typename... Args>
		static void call(BrokerType& broker, Args&&... args)
		{
			ComponentType::onCall(TopicType(), broker, std::get<BrokerType::template ComponentIndex<ComponentType>::value>(broker.m_cores), std::forward<Args>(args)...);
		}
	};

	/**
	 * @brief The class that connects the components together.
	 * Register the components using the component() function. Then,
	 * notify() to notification topics, publish() to publication topics
	 * and call() service topics.
	 * 
	 * @tparam CoreTypes The components.
	 */
	template<typename... ComponentTypes>
	class Broker
	{
		template<typename...> friend class Subscribe;
		template<typename> friend class Provide;

		using ComponentsTuple = std::tuple<ComponentTypes...>;
		using CoreRefs = std::tuple<typename ComponentTypes::Core&...>;

		template<typename ComponentType, std::size_t IndexValue=0>
		struct ComponentIndex
		{
			static constexpr auto Size = std::tuple_size<ComponentsTuple>::value;
			static constexpr std::size_t value = IndexValue==(Size-1)? Size : ComponentIndex<ComponentType, IndexValue+1>::value;
		};
		template<std::size_t IndexValue>
		struct ComponentIndex<typename std::tuple_element<IndexValue, ComponentsTuple>::type, IndexValue> 
		{
			static constexpr std::size_t value = IndexValue;
		};

	public:
		class Builder
		{
			using CorePtrs = std::tuple<typename ComponentTypes::Core*...>;

			Builder() = default;

		/**
		 * @brief This registers a core for a component.
		 * 
		 * @tparam ComponentType The component for which the core will be
		 * registered.
		 * @param component The core to register.
		 */
			template<typename ComponentType>
			void set(typename ComponentType::Core& core)
			{
				std::get<ComponentIndex<ComponentType>::value>(m_cores) = &core;
			}

			Broker build() const
			{
				return Broker(*std::get<ComponentIndex<ComponentTypes>::value>(m_cores)...);
			}

		private:
			CorePtrs m_cores;
		};

		Broker(typename ComponentTypes::Core&... cores):
			m_cores(cores...)
		{}

		/**
		 * @brief Call the subscribed notification and publication
		 * callbacks for TopicType.
		 * 
		 * @tparam TopicType The topic.
		 * @tparam Args Deduced from args.
		 * @param args Const ref arguments to the callbacks.
		 */
		template<typename TopicType, typename... Args>
		void publish(Args&&... args)
		{
			Topic<TopicType>::template publish<TopicType>(*this, std::forward<const Args>(args)...);
		}
		
		/**
		 * @brief Call the service callback from TopicType's provider.
		 * 
		 * @tparam TopicType The topic.
		 * @tparam Args Deduced from args.
		 * @param args Arguments forwarded to the callback.
		 * @return Return<TopicType> The possibly void return value.
		 */
		template<typename TopicType, typename... Args>
		auto call(Args&&... args) -> decltype(Topic<TopicType>::template call<TopicType>(*this, std::forward<Args>(args)...))
		{
			return Topic<TopicType>::template call<TopicType>(*this, std::forward<Args>(args)...);
		}

	private:
		/// Refrences to the cores
		const CoreRefs m_cores;
	};
}
