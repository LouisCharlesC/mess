/**
 * @file broker.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2018-11-01
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef BROKER_H_
#define BROKER_H_

#include "indicestrick.h"
#include "state.hpp"
#include "topictraits.h"

#include <cstdint>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

// TODO:	come on, remove vectors!
//				allow moving in even without pull capability
//				allow saving the other arguments for non std::function callbacks

namespace mess {
	template<typename TopicGroup>
	class UnitBroker
	{
		static_assert(Count<TopicGroup>() != 0, "Count for TopicGroup must be non-zero");

		template<TopicGroup Topic, bool>
		struct TopicInfo {};
		template<TopicGroup Topic>
		struct TopicInfo<Topic, false>
		{
			std::vector<CallbackType<TopicGroup, Topic>> callbacks;
		};
		template<TopicGroup Topic>
		struct TopicInfo<Topic, true>
		{
			std::vector<CallbackType<TopicGroup, Topic>> callbacks;
			mutable State<MessageType<TopicGroup, Topic>> cache;
		};

		template<std::size_t Index, typename... Elements>
		struct MakeBillboard
		{
			using type = typename MakeBillboard<Index+1, Elements..., TopicInfo<static_cast<TopicGroup>(Index), IsPullEnabled<TopicGroup, static_cast<TopicGroup>(Index)>()>>::type;
		};
		template<typename... Elements>
		struct MakeBillboard<Count<TopicGroup>(), Elements...>
		{
			using type = std::tuple<Elements...>;
		};
		using Billboard = typename MakeBillboard<0>::type;

	public:
		template<TopicGroup Topic, typename... Args>
		void subscribe(Args&&... args)
		{
			std::get<static_cast<std::size_t>(Topic)>(m_billboard).callbacks.emplace_back(std::forward<Args>(args)...);
		}

		template<TopicGroup Topic>
		void notify() const
		{
			static_assert(std::is_same<MessageType<TopicGroup, Topic>, void>::value, "notify only for topics with message type == void");
			for (const auto& callback: std::get<static_cast<std::size_t>(Topic)>(m_billboard).callbacks)
			{
				callback();
			}
		}

		template<TopicGroup Topic>
		void push(const MessageType<TopicGroup, Topic>& message) const
		{
			call<Topic>(message);
			cache<Topic>(message, PullCapability<TopicGroup, Topic>());
		}
		template<TopicGroup Topic>
		void push(MessageType<TopicGroup, Topic>&& message) const
		{
			call<Topic>(message);
			cache<Topic>(std::move(message), PullCapability<TopicGroup, Topic>());
		}

		template<TopicGroup Topic>
		const State<MessageType<TopicGroup, Topic>>& pull() const
		{
			static_assert(!std::is_same<MessageType<TopicGroup, Topic>, void>::value, "Can't pull topic with message type == void");
			static_assert(IsPullEnabled<TopicGroup, Topic>(), "Enable pull by specializing PullCapability for your topic.");
			return std::get<static_cast<std::size_t>(Topic)>(m_billboard).cache;
		}

		template<TopicGroup Topic>
		bool hasSubscribers() const
		{
			return !std::get<static_cast<std::size_t>(Topic)>(m_billboard).callbacks.empty();
		}

	private:
		template<TopicGroup Topic>
		void call(const MessageType<TopicGroup, Topic>& message) const
		{
			for (const auto& callback: std::get<static_cast<std::size_t>(Topic)>(m_billboard).callbacks)
			{
				callback(message);
			}
		}
		template<TopicGroup Topic>
		void cache(const MessageType<TopicGroup, Topic>& message, PullDisabled_t) const
		{}
		template<TopicGroup Topic>
		void cache(const MessageType<TopicGroup, Topic>& message, PullEnabled_t) const
		{
			std::get<static_cast<std::size_t>(Topic)>(m_billboard).cache.set(message);
		}
		template<TopicGroup Topic>
		void cache(MessageType<TopicGroup, Topic>&& message, PullEnabled_t) const
		{
			std::get<static_cast<std::size_t>(Topic)>(m_billboard).cache.set(std::move(message));
		}

		Billboard m_billboard;
	};

	template<typename... BrokerTopicGroups>
	class Broker
	{
		static_assert(sizeof...(BrokerTopicGroups) >= 1, "");
		
		using UnitBrokers = std::tuple<UnitBroker<BrokerTopicGroups>&...>;

	public:
		Broker(const UnitBrokers& brokers):
			m_brokers(brokers)
		{}

		template<typename TopicGroup, TopicGroup Topic, typename... Args>
		void subscribe(Args&&... args)
		{
			get_broker<TopicGroup>().template subscribe<Topic>(std::forward<Args>(args)...);
		}

		template<typename TopicGroup, TopicGroup Topic>
		void notify() const
		{
			get_broker<TopicGroup>().template notify<Topic>();
		}

		template<typename TopicGroup, TopicGroup Topic>
		void push(const MessageType<TopicGroup, Topic>& message) const
		{
			get_broker<TopicGroup>().template push<Topic>(message);
		}
		template<typename TopicGroup, TopicGroup Topic>
		void push(MessageType<TopicGroup, Topic>&& message) const
		{
			get_broker<TopicGroup>().template push<Topic>(std::move(message));
		}

		template<typename TopicGroup, TopicGroup Topic>
		const State<MessageType<TopicGroup, Topic>>& pull() const
		{
			return get_broker<TopicGroup>().template pull<Topic>();
		}

		template<typename TopicGroup, TopicGroup Topic>
		bool hasSubscribers() const
		{
			return get_broker<TopicGroup>().template hasSubscribers<Topic>();
		}

	private:
		template<typename TopicGroup>
		UnitBroker<TopicGroup>& get_broker() const
		{
			return std::get<tricks::FullSet<BrokerTopicGroups...>::template MakeIndices<TopicGroup>::index>(m_brokers);
		}

		UnitBrokers m_brokers;
	};
	
	template<typename... MasterTopicGroups>
	class MasterBroker
	{
		static_assert(sizeof...(MasterTopicGroups) >= 1, "");

		using UnitBrokers = std::tuple<UnitBroker<MasterTopicGroups>...>;

	public:
		template<typename... BrokerTopicGroups>
		Broker<BrokerTopicGroups...> make_broker()
		{
			return Broker<BrokerTopicGroups...>(select_brokers(typename tricks::FullSet<MasterTopicGroups...>::template MakeIndices<BrokerTopicGroups...>()));
		}

	private:
		template<std::size_t... Indices>
		std::tuple<typename std::tuple_element<Indices, UnitBrokers>::type&...> select_brokers(const tricks::indices<Indices...>&)
		{
			return std::forward_as_tuple(std::get<Indices>(m_brokers)...);
		}

		UnitBrokers m_brokers;
	};

}  // namespace mess

#endif /* BROKER_H_ */
