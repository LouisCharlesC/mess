/*
 * broker.h
 *
 *  Created on: Nov. 1, 2018
 *      Author: lcc
 */

#ifndef BROKER_H_
#define BROKER_H_

#include "messagetraits.h"

#include <cstdint>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

// TODO: 1. There could be support for a Broker with specific topics, rather than topics from 0 to NbrOfTopics-1

namespace mess {
	template<std::size_t NbrOfTopics>
	class Broker
	{
		template<std::size_t Topic>
		using Callback = std::function<void(const PayloadType<Topic>&)>;

		template<std::size_t Topic>
		struct TopicInfo
		{
			std::vector<Callback<Topic>> callbacks;
		};

		template<std::size_t Index, std::size_t Size, typename... Elements>
		struct MakeBillboard
		{
			static_assert(Size != 0, "no good");
			using type = typename MakeBillboard<Index+1, Size, Elements..., TopicInfo<Index>>::type;
		};
		template<std::size_t Size, typename... Elements>
		struct MakeBillboard<Size, Size, Elements...>
		{
			using type = std::tuple<Elements...>;
		};

		template<std::size_t Size>
		using Billboard = typename MakeBillboard<0, Size>::type;

	public:
		template<std::size_t Topic, typename... Args>
    void subscribe(Args&&... args)
    {
			checkIndex<Topic>();
			std::get<Topic>(m_billboard).callbacks.emplace_back(std::forward<Args>(args)...);
    }

		template<std::size_t Topic>
		void publish(const PayloadType<Topic>& message) const
		{
			checkIndex<Topic>();
			for (const auto& callback: std::get<Topic>(m_billboard).callbacks)
			{
				callback(message);
			}
		}

		template<std::size_t Topic>
		std::size_t getNbrOfSubscribers() const
		{
			checkIndex<Topic>();
			return std::get<Topic>(m_billboard).callbacks.size();
		}

	private:
		template<std::size_t Topic>
		void checkIndex() const
		{
			static_assert(Topic<NbrOfTopics, "Function template argument Topic must be lower than class template argument NbrOfTopics");
		}

		Billboard<NbrOfTopics> m_billboard;
	};
}  // namespace mess

#endif /* BROKER_H_ */
