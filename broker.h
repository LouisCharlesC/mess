/*
 * broker.h
 *
 *  Created on: Sep 12, 2018
 *      Author: lcc
 */

#ifndef BROKER_H_
#define BROKER_H_

#include <functional>
#include <memory>
#include <tuple>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

namespace mess
{
	template<typename PayloadType>
	using PersistentPublicationCallback = std::function<const PayloadType&()>;
	template<typename PayloadType>
	using ConstSubscriptionCallback = std::function<void(const PayloadType&)>;
	template<typename PayloadType>
	using MutatingSubscriptionCallback = std::function<void(PayloadType&&)>;

	template<typename TopicUIDType>
	class Broker
	{
	public:
		template<typename PayloadType>
		void publish(const TopicUIDType& persistentTopic, const PersistentPublicationCallback<PayloadType>& callback) noexcept
		{
			m_billboard[typeid(PayloadType).hash_code()][persistentTopic].persistentPublicationCallback = std::bind(&anonymousPersistentPublicationCallback<PayloadType>, callback);
			publish(persistentTopic, callback());
		}

		template<typename PayloadType>
		void publish(const TopicUIDType& persistentTopic) const
		{
			const auto callbacks = getCallbacks<PayloadType>(persistentTopic);
			if (callbacks)
			{
				if (callbacks->second.persistentPublicationCallback)
				{
					publish(persistentTopic, *static_cast<const PayloadType*>(callbacks->second.persistentPublicationCallback()));
				}
			}
			else
			{
				throw std::logic_error("[Broker::publish] Called publish on topic " + std::to_string(persistentTopic) + " before registering a persistent publication callback.");
			}
		}

		template<typename PayloadType>
		void publish(const TopicUIDType& volatileTopic, const PayloadType& message) const
		{
			const auto callbacks = getCallbacks<PayloadType>(volatileTopic);
			if (callbacks)
			{
				dispatchConst(callbacks->constSubscriptionCallbacks, &message);

				if (!callbacks->mutatingSubscriptionCallbacks.empty())
				{
					dispatchMutating(callbacks->mutatingSubscriptionCallbacks, PayloadType(message));
				}
			}
		}

		template<typename PayloadType>
		void publish(const TopicUIDType& volatileTopic, PayloadType&& message) const
		{
			const auto callbacks = getCallbacks<PayloadType>(volatileTopic);
			if (callbacks)
			{
				dispatchConst(callbacks->constSubscriptionCallbacks, &message);

				if (!callbacks->mutatingSubscriptionCallbacks.empty())
				{
					dispatchMutating(callbacks->mutatingSubscriptionCallbacks, std::move(message));
				}
			}
		}

		template<typename PayloadType>
		void subscribe(const TopicUIDType& topic, const ConstSubscriptionCallback<PayloadType>& callback)
		{
			m_billboard[typeid(PayloadType).hash_code()][topic].constSubscriptionCallbacks.push_back(std::bind(&anonymousConstSubscriptionCallback<PayloadType>, callback, std::placeholders::_1));
		}

		template<typename PayloadType>
		void subscribe(const TopicUIDType& topic, const MutatingSubscriptionCallback<PayloadType>& callback)
		{
			m_billboard[typeid(PayloadType).hash_code()][topic].mutatingSubscriptionCallbacks.push_back(std::bind(&anonymousMutatingSubscriptionCallback<PayloadType>, callback, std::placeholders::_1));
		}

		template<typename PayloadType>
		bool exists(const TopicUIDType& topic) const
		{
			return getCallbacks<PayloadType>(topic);
		}

		template<typename PayloadType>
		bool isPersistent(const TopicUIDType& topic) const
		{
			return getCallbacks<PayloadType>(topic)->persistentPublicationCallback;
		}

		template<typename PayloadType>
		bool hasSubscribers(const TopicUIDType& topic) const
		{
			const auto callbacks = getCallbacks<PayloadType>(topic);
			if (callbacks)
			{
				return !callbacks->constSubscriptionCallbacks.empty() || !callbacks->mutatingSubscriptionCallbacks.empty();
			}
			else
			{
				return false;
			}
		}

	private:
		using AnyPersistentPublicationCallback = std::function<const void*()>;
		using AnyConstSubscriptionCallback = std::function<void(const void*)>;
		using AnyMutatingSubscriptionCallback = std::function<void(void*)>;

		struct CallbacksForATopic
		{
			AnyPersistentPublicationCallback persistentPublicationCallback;
			std::vector<AnyConstSubscriptionCallback> constSubscriptionCallbacks;
			std::vector<AnyMutatingSubscriptionCallback> mutatingSubscriptionCallbacks;
		};

		template<typename PayloadType>
		const CallbacksForATopic* getCallbacks(const TopicUIDType& topic) const
		{
			const auto itTopics = m_billboard.find(typeid(PayloadType).hash_code());
			if (itTopics != m_billboard.cend())
			{
				const auto itCallbacks = itTopics->second.find(topic);
				if (itCallbacks != itTopics->second.cend())
				{
					return &itCallbacks->second;
				}
			}
			return nullptr;
		}

		template<typename PayloadType>
		static const void* anonymousPersistentPublicationCallback(const PersistentPublicationCallback<PayloadType>& callback)
		{
			return &callback();
		}

		template<typename PayloadType>
		static void anonymousConstSubscriptionCallback(const ConstSubscriptionCallback<PayloadType>& callback, const void* message)
		{
			callback(*static_cast<const PayloadType*>(message));
		}

		template<typename PayloadType>
		static void anonymousMutatingSubscriptionCallback(const MutatingSubscriptionCallback<PayloadType>& callback, void* message)
		{
			callback(std::move(*static_cast<PayloadType*>(message)));
		}

		static void dispatchConst(const std::vector<AnyConstSubscriptionCallback>& callbacks, const void* message)
		{
			for (const auto& callback: callbacks)
			{
				callback(message);
			}
		}

		/*
		 * Do not call if callbacks is empty!
		 */
		template<typename PayloadType>
		static void dispatchMutating(const std::vector<AnyMutatingSubscriptionCallback>& callbacks, PayloadType&& message)
		{
			auto itCallback = callbacks.cbegin();
			for (; itCallback != callbacks.cend()-1; ++ itCallback)
			{
				PayloadType copy = message;
				(*itCallback)(&copy);
			}
			(*itCallback)(&message);
		}

		std::unordered_map<std::size_t, std::unordered_map<TopicUIDType, CallbacksForATopic>> m_billboard;
	};
}  // namespace mess

#endif /* BROKER_H_ */
