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

#include "messtraits.h"
#include "state.hpp"

#include <cassert>
#include <cstddef>
#include <tuple>
#include <type_traits>

namespace mess
{
	template<typename Topic, typename Subscriber>
	void onNotify(Subscriber&)
	{}
	template<typename Topic, typename Subscriber>
	void onPublish(Subscriber&, const Message<Topic>&)
	{}
	template<typename Topic, typename Sink>
	void onMove(Sink&, Message<Topic>&&)
	{}
	template<typename Service, typename... Args>
	Return<Service> onCall(Provider<Service>&, Args...)
	{}

	template<typename... SubscribersTypes>
	class BrokerTemplate
	{
		using SubscriberTypesTuple = std::tuple<SubscribersTypes...>;
		using SubscriberPtrsTuple = std::tuple<SubscribersTypes*...>;

	public:
		BrokerTemplate() = delete;

		template<typename SubscribersEnum, SubscribersEnum Subscriber>
		static void subscriber(typename std::tuple_element<static_cast<std::size_t>(Subscriber), SubscriberTypesTuple>::type& ref)
		{
			subscriber<static_cast<std::size_t>(Subscriber)>(ref);
		}
		template<std::size_t SubscriberIndex>
		static void subscriber(typename std::tuple_element<SubscriberIndex, SubscriberTypesTuple>::type& ref)
		{
			assert(std::get<SubscriberIndex>(m_subscribers) == nullptr);
			std::get<SubscriberIndex>(m_subscribers) = &ref;
		}

		template<typename Topic>
		static void publish(const Message<Topic>& message)
		{
			publish<Topic>(std::integral_constant<bool, TopicTraits<Topic>::IsCacheEnabled>(), message);
		}
		template<typename Topic>
		static void publish(Message<Topic>&& message)
		{
			publish<Topic>(std::integral_constant<bool, TopicTraits<Topic>::IsCacheEnabled>(), std::move(message));
		}

		template<typename Topic>
		static void notify()
		{
			dispatcher<>::template notify<Topic>();
		}
		
		template<typename Topic>
		static const State<Message<Topic>>& cache()
		{
			return getCache<Topic>();
		}

		// template<typename Topic>
		// static move(Message<Topic>&& message)
		// {
		// 	return caller<Topic>::call();
		// }

		template<typename Service, typename... Args>
		static Return<Service> call(Args&&... args)
		{
			return caller<Service>::call(std::forward<Args>(args)...);
		}

	private:
		template<typename Topic>
		static void publish(std::true_type, const Message<Topic>& message)
		{
			dispatcher<>::template publish<Topic>(message);
			getCache<Topic>().set(message);
			dispatcher<>::template notify<Topic>();
		}
		template<typename Topic>
		static void publish(std::true_type, Message<Topic>&& message)
		{
			dispatcher<>::template publish<Topic>(message);
			getCache<Topic>().set(std::move(message));
			dispatcher<>::template notify<Topic>();
		}
		template<typename Topic>
		static void publish(std::false_type, const Message<Topic>& message)
		{
			dispatcher<>::template publish<Topic>(message);
			dispatcher<>::template notify<Topic>();
		}

		template<bool=true, std::size_t CurrentIndex=0>
		struct dispatcher
		{
			template<typename Topic>
			static void publish(const Message<Topic>& message)
			{
				assert(std::get<CurrentIndex>(m_subscribers) != nullptr);
				::mess::onPublish<Topic>(*std::get<CurrentIndex>(m_subscribers), message);
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
			static void publish(const Message<Topic>& message)
			{}
			template<typename Topic>
			static void notify()
			{}
		};

		template<typename Topic>
		static State<Message<Topic>>& getCache()
		{
			static_assert(TopicTraits<Topic>::IsCacheEnabled, "Specialize TopicTraits and set IsCacheEnabled to true to enable caching for your topic.");
			static State<Message<Topic>> cache(TopicTraits<Topic>::InitialValue);
			return cache;
		}

		template<typename Service, std::size_t CurrentIndex=0, typename Provider=Provider<Service>>
		struct caller
		{
			template<typename... Args>
			static Return<Service> call(Args&&... args)
			{
				return caller<Service, CurrentIndex+1>::call(std::forward<Args>(args)...);
			}
		};
		template<typename Service, std::size_t CurrentIndex>
		struct caller<Service, CurrentIndex, typename std::tuple_element<CurrentIndex, SubscriberTypesTuple>::type>
		{
			template<typename... Args>
			static Return<Service> call(Args&&... args)
			{
				return onCall<Service>(*std::get<CurrentIndex>(m_subscribers), std::forward<Args>(args)...);
			}
		};

		static SubscriberPtrsTuple m_subscribers;
	};

	template<typename... SubscribersTypes>
	typename BrokerTemplate<SubscribersTypes...>::SubscriberPtrsTuple BrokerTemplate<SubscribersTypes...>::m_subscribers;
}

