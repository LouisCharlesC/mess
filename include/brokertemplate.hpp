/**
 * @file staticbroker.hpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "state.hpp"

#include <cassert>
#include <cstddef>
#include <tuple>
#include <type_traits>

// TODO:	count subscribers ?
//				allow asking the template for the subscribers types ?
//				allow notify ?
//				allow service calls ?

namespace mess
{
	template<typename Topic, typename Subscriber>
	void onPublish(const typename Topic::Message&, Subscriber& subscriber)
	{}
	// template<typename Topic, typename Subscriber>
	// void onNotify(Subscriber& subscriber)
	// {}

	template<typename... SubscribersTypes>
	class BrokerTemplate
	{
		using Subscribers = std::tuple<SubscribersTypes...>;
		using SubscribersPtrs = std::tuple<SubscribersTypes*...>;
		template<typename MessageType>
		using CacheType = State<MessageType>;

	public:
		BrokerTemplate() = delete;

		template<typename SubscribersEnum, SubscribersEnum Subscriber>
		static void subscriber(typename std::tuple_element<static_cast<std::size_t>(Subscriber), Subscribers>::type& ref)
		{
			assert(std::get<static_cast<std::size_t>(Subscriber)>(m_subscribers) == nullptr);
			std::get<static_cast<std::size_t>(Subscriber)>(m_subscribers) = &ref;;
		}
		template<std::size_t SubscriberIndex>
		static void subscriber(typename std::tuple_element<SubscriberIndex, Subscribers>::type& ref)
		{
			assert(std::get<SubscriberIndex>(m_subscribers) == nullptr);
			std::get<SubscriberIndex>(m_subscribers) = &ref;
		}

		// template<typename Service, typename... Args>
		// static typename Service::ReturnType call(Args&&... args)
		// {

		// }
		
		// template<typename Topic>
		// static void notify()
		// {
		// 	notify<Topic>(build_indices<sizeof...(SubscribersTypes)>());
		// }
		
		template<typename Topic>
		static void publish(const typename Topic::Message& message)
		{
			publish<Topic>(std::integral_constant<bool, Topic::IsCacheEnabled>(), message, build_indices<sizeof...(SubscribersTypes)>());
		}
		
		template<typename Topic>
		static const CacheType<typename Topic::Message>& cache()
		{
			return getCache<Topic>();
		}

	private:
		template<typename Topic>
		static CacheType<typename Topic::Message>& getCache()
		{
			static_assert(Topic::IsCacheEnabled, "Set IsCacheEnabled to true to enable caching for your topic.");
			static CacheType<typename Topic::Message> cache;
			return cache;
		}

	  template <std::size_t... Indices>
		struct indices {};

		template <std::size_t To, std::size_t Current=0, std::size_t... Indices>
		struct build_indices : build_indices<To, Current+1, Indices..., Current> {};

		template <std::size_t To, std::size_t... Indices>
		struct build_indices<To, To, Indices...> : indices<Indices...> {};

		template<typename Topic, std::size_t... Indices>
		static void publish(std::true_type, const typename Topic::Message& message, indices<Indices...>)
		{
			getCache<Topic>().set(message);
			publish<Topic>(std::false_type(), message, indices<Indices...>());
		}
		template<typename Topic, std::size_t... Indices>
		static void publish(std::false_type, const typename Topic::Message& message, indices<Indices...>)
		{
			publisher<Indices...>::template publish<Topic>(message);
		}

		template<std::size_t CurrentIndex, std::size_t... OtherIndices>
		struct publisher
		{
			template<typename Topic>
			static void publish(const typename Topic::Message& message)
			{
				assert(std::get<CurrentIndex>(m_subscribers) != nullptr);
				publisher<CurrentIndex>::template publish<Topic>(message);
				publisher<OtherIndices...>::template publish<Topic>(message);
			}
		};
		template<std::size_t CurrentIndex>
		struct publisher<CurrentIndex>
		{
			template<typename Topic>
			static void publish(const typename Topic::Message& message)
			{
				::mess::onPublish<Topic>(message, *std::get<CurrentIndex>(m_subscribers));
			}
		};

		// static SubscribersPtrs initialize()
		// {
		// 	SubscribersPtrs subscribers;
		// 	initialize(subscribers, build_indices<sizeof...(SubscribersTypes)>());
		// 	return subscribers;
		// }
		// template<std::size_t... Indices>
		// static void initialize(SubscribersPtrs& subscribers, indices<Indices...>)
		// {
		// 	initializer<Indices...>::initialize(subscribers);
		// }

		// template<std::size_t CurrentIndex, std::size_t... OtherIndices>
		// struct initializer
		// {
		// 	static void initialize(SubscribersPtrs& subscribers)
		// 	{
		// 		initializer<CurrentIndex>::initialize(subscribers);
		// 		initializer<OtherIndices...>::initialize(subscribers);
		// 	}
		// };
		// template<std::size_t CurrentIndex>
		// struct initializer<CurrentIndex>
		// {
		// 	static void initialize(SubscribersPtrs& subscribers)
		// 	{
		// 		std::get<CurrentIndex>(subscribers) = ;
		// 	}
		// };

		static SubscribersPtrs m_subscribers;
	};

	template<typename... SubscribersTypes>
	typename BrokerTemplate<SubscribersTypes...>::SubscribersPtrs BrokerTemplate<SubscribersTypes...>::m_subscribers;// = BrokerTemplate<SubscribersTypes...>::initialize();
}

