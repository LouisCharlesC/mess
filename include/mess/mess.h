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
			// comment the code

namespace mess
{
	/**
	 * @brief Base trait class to define topics.
	 * 
	 * @tparam TopicType The topic.
	 */
	template<typename TopicType>
	struct Topic;

	template<typename FirstComponentType, typename... OtherComponentTypes>
	struct Subscribers
	{
		template<typename TopicType, typename BrokerType, typename... Args>
		static void publish(const BrokerType& broker, Args&&... args)
		{
			// Sorry for this line of code, it is explained here: https://arne-mertz.de/2016/11/more-variadic-templates/
			// It just calls the onPublish member function from all subcribing components, forwarding args on each call
			// I do not know if this form for the call to std::forward: "std::forward<const Args>(args)..." (notice the const) is right, but it produces result I am looking for: arguments are forwarded without ever being moved.
			(void)std::initializer_list<int>{ // gather all the below generated ints in an unused initializer_list
				(OtherComponentTypes::onPublish( // call onPublish
					TopicType(), // pass the topic
					broker, // pass the broker
					std::get<BrokerType::template ComponentIndex<OtherComponentTypes>::value>(broker.m_cores), // pass the core
					std::forward<const Args>(args)...) // forward the const-ified arguments, 
				, 0) // tricky: this calls the above onPublish function, but returns 0 rather than void
				..., // do the above for all OtherComponentTypes
				(FirstComponentType::onPublish( // do the above for the FirstComponentType
					TopicType(),
					broker,
					std::get<BrokerType::template ComponentIndex<FirstComponentType>::value>(broker.m_cores),
					std::forward<Args>(args)...) // forward the arguments as-is
				, 0)
			};
		}
	};

	template<typename Type>
	struct Return
	{
		using ReturnType = Type;
	};
	template<typename FirstComponentType, typename... OtherComponentTypes>
	struct Providers
	{
		template<typename TopicType, typename BrokerType, typename... Args>
		static std::array<typename Topic<TopicType>::Return::ReturnType, sizeof...(OtherComponentTypes)+1> call(const BrokerType& broker, Args&&... args)
		{
			return {
				OtherComponentTypes::onCall(
					TopicType(),
					broker,
					std::get<BrokerType::template ComponentIndex<OtherComponentTypes>::value>(broker.m_cores),
					std::forward<const Args>(args)...
				)...,
				Providers<FirstComponentType>::template call<TopicType>(broker, std::forward<Args>(args)...)
			};
		}
	};
	template<typename ComponentType>
	struct Providers<ComponentType>
	{
		template<typename TopicType, typename BrokerType, typename... Args>
		static typename Topic<TopicType>::Return::ReturnType call(const BrokerType& broker, Args&&... args)
		{
			return ComponentType::onCall(
				TopicType(),
				broker,
				std::get<BrokerType::template ComponentIndex<ComponentType>::value>(broker.m_cores),
				std::forward<Args>(args)...
			);
		}
	};

	template<typename... ComponentTypes>
	class Broker
	{
		template<typename, typename...> friend class Subscribers;
		template<typename, typename...> friend class Providers;

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

		public:
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
		void publish(Args&&... args) const
		{
			Topic<TopicType>::Subscribers::template publish<TopicType>(*this, std::forward<Args>(args)...);
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
		auto call(Args&&... args) const -> decltype(Topic<TopicType>::Providers::template call<TopicType>(*this, std::forward<Args>(args)...))
		{
			return Topic<TopicType>::Providers::template call<TopicType>(*this, std::forward<Args>(args)...);
		}

	private:
		/// Refrences to the cores
		const CoreRefs m_cores;
	};
}
