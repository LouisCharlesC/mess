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

#include <cstddef>
#include <tuple>
#include <type_traits>



namespace mess
{
	template<typename Topic, typename Subscriber>
	void onPublish(const typename Topic::Message&, Subscriber& subscriber)
	{}

	template<typename... SubscribersTypes>
	class BrokerTemplate
	{
		using SubscribersPtrs = std::tuple<SubscribersTypes*...>;

	public:
		BrokerTemplate() = delete;

		template<typename SubscribersEnum, SubscribersEnum Subscriber>
		static typename std::tuple_element<static_cast<std::size_t>(Subscriber), SubscribersPtrs>::type& subscriber()
		{
			return std::get<static_cast<std::size_t>(Subscriber)>(subscribers);
		}
		template<std::size_t SubscriberIndex>
		static typename std::tuple_element<SubscriberIndex, SubscribersPtrs>::type& subscriber()
		{
			return std::get<SubscriberIndex>(subscribers);
		}
		
		template<typename Topic>
		static void publish(const typename Topic::Message& message)
		{
			publish<Topic>(message, build_indices<sizeof...(SubscribersTypes)>());
		}

	private:
	  template <std::size_t... Indices>
		struct indices {};

		template <std::size_t To, std::size_t Current=0, std::size_t... Indices>
		struct build_indices : build_indices<To, Current+1, Indices..., Current> {};

		template <std::size_t To, std::size_t... Indices>
		struct build_indices<To, To, Indices...> : indices<Indices...> {};

		template<typename Topic, std::size_t FirstIndex, std::size_t... OtherIndices>
		static void publish(const typename Topic::Message& message, indices<FirstIndex, OtherIndices...>)
		{
			publisher<FirstIndex, OtherIndices...>::template publish<Topic>(message);
		}

		template<std::size_t CurrentIndex, std::size_t... OtherIndices>
		struct publisher
		{
			template<typename Topic>
			static void publish(const typename Topic::Message& message)
			{
				::mess::onPublish<Topic>(message, *std::get<CurrentIndex>(subscribers));
				publisher<OtherIndices...>::template publish<Topic>(message);
			}
		};
		template<std::size_t LastIndex>
		struct publisher<LastIndex>
		{
			template<typename Topic>
			static void publish(const typename Topic::Message& message)
			{
				::mess::onPublish<Topic>(message, *std::get<LastIndex>(subscribers));
			}
		};

		static SubscribersPtrs subscribers;
	};

	template<typename... SubscribersTypes>
	typename BrokerTemplate<SubscribersTypes...>::SubscribersPtrs BrokerTemplate<SubscribersTypes...>::subscribers;
}

#define MESS_SUBSCRIBE_TEMPLATE(Topic, Subscriber) \
namespace mess \
{ \
	template<> \
	void onPublish<Topic, Subscriber>(const Topic::Message& message, Subscriber& subscriber) \
	{ \
		subscriber.onPublish<Topic>(message); \
	} \
}

#define MESS_SUBSCRIBE_TAG(Topic, Subscriber) \
namespace mess \
{ \
	template<> \
	void onPublish<Topic, Subscriber>(const Topic::Message& message, Subscriber& subscriber) \
	{ \
		subscriber.onPublish(message, Topic()); \
	} \
}

