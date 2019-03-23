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
//				detect multiple service providers ? allow multiple providers and return an array ?

namespace mess
{
	template<typename Topic, typename Subscriber>
	void onPublish(const typename Topic::Message&, Subscriber& subscriber)
	{}
	template<typename Topic, typename Subscriber>
	void onNotify(Subscriber& subscriber)
	{}

	template<typename... SubscribersTypes>
	class BrokerTemplate
	{
		using SubscribersTuple = std::tuple<SubscribersTypes...>;
		using SubscribersPtrs = std::tuple<SubscribersTypes*...>;
		template<typename MessageType>
		using CacheType = State<MessageType>;

	public:
		BrokerTemplate() = delete;

		template<typename SubscribersEnum, SubscribersEnum Subscriber>
		static void subscriber(typename std::tuple_element<static_cast<std::size_t>(Subscriber), SubscribersTuple>::type& ref)
		{
			subscriber<static_cast<std::size_t>(Subscriber)>(ref);
		}
		template<std::size_t SubscriberIndex>
		static void subscriber(typename std::tuple_element<SubscriberIndex, SubscribersTuple>::type& ref)
		{
			assert(std::get<SubscriberIndex>(m_subscribers) == nullptr);
			std::get<SubscriberIndex>(m_subscribers) = &ref;
		}

		template<typename Topic>
		static void publish(const typename Topic::Message& message)
		{
			publish<Topic>(std::integral_constant<bool, Topic::IsCacheEnabled>(), message);
		}
		template<typename Topic>
		static void publish(typename Topic::Message&& message)
		{
			publish<Topic>(std::integral_constant<bool, Topic::IsCacheEnabled>(), std::move(message));
		}
		template<typename Topic>
		static void notify()
		{
			dispatcher<>::template notify<Topic>();
		}
		template<typename Service, typename... Args>
		static typename Service::ReturnType call(Args&&... args)
		{
			return caller<Service>::call(args...);
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

		template<typename Topic>
		static void publish(std::true_type, const typename Topic::Message& message)
		{
			publish<Topic>(std::false_type(), message);
			getCache<Topic>().set(message);
		}
		template<typename Topic>
		static void publish(std::false_type, const typename Topic::Message& message)
		{
			dispatcher<>::template publish<Topic>(message);
		}
		template<typename Topic>
		static void publish(std::true_type, typename Topic::Message&& message)
		{
			publish<Topic>(std::false_type(), message);
			getCache<Topic>().set(std::move(message));
		}

		template<bool=true, std::size_t CurrentIndex=0>
		struct dispatcher
		{
			template<typename Topic>
			static void publish(const typename Topic::Message& message)
			{
				assert(std::get<CurrentIndex>(m_subscribers) != nullptr);
				::mess::onPublish<Topic>(message, *std::get<CurrentIndex>(m_subscribers));
				dispatcher<true, CurrentIndex+1>::template publish<Topic>(message);
			}
			template<typename Topic>
			static void notify()
			{
				assert(std::get<CurrentIndex>(m_subscribers) != nullptr);
				::mess::onNotify<Topic>(*std::get<CurrentIndex>(m_subscribers));
				dispatcher<true, CurrentIndex+1>::template notify<Topic>();
			}
		};
		template<bool dummy>
		struct dispatcher<dummy, sizeof...(SubscribersTypes)>
		{
			template<typename Topic>
			static void publish(const typename Topic::Message& message)
			{}
			template<typename Topic>
			static void notify()
			{}
		};

		template<typename Service, std::size_t CurrentIndex=0, typename Provider=typename Service::Provider>
		struct caller
		{
			template<typename... Args>
			static typename Service::ReturnType call(Args&&... args)
			{
				return caller<Service, CurrentIndex+1>::call(std::forward<Args>(args)...);
			}
		};
		template<typename Service, std::size_t CurrentIndex>
		struct caller<Service, CurrentIndex, typename std::tuple_element<CurrentIndex, SubscribersTuple>::type>
		{
			template<typename... Args>
			static typename Service::ReturnType call(Args&&... args)
			{
				return std::get<CurrentIndex>(m_subscribers)->onCall(std::forward<Args>(args)..., Service());
			}
		};

		static SubscribersPtrs m_subscribers;
	};

	template<typename... SubscribersTypes>
	typename BrokerTemplate<SubscribersTypes...>::SubscribersPtrs BrokerTemplate<SubscribersTypes...>::m_subscribers;
}

