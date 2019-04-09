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

#include "traits.h"

#include <cassert>
#include <cstddef>
#include <tuple>
#include <type_traits>

namespace mess
{
	/**
	 * @brief The class that connects the components together.
	 * Register the components using the component() function. Then,
	 * notify() to notification topics, publish() to publication topics
	 * and call() service topics.
	 * 
	 * @tparam ComponentTypes The components.
	 */
	template<typename... ComponentTypes>
	class BrokerTemplate
	{
		using Components = std::tuple<ComponentTypes...>;
		using ComponentPtrs = std::tuple<ComponentTypes*...>;

	public:
		BrokerTemplate() = default;

		/**
		 * @brief This registers a component, allowing its callbacks to be
		 * called by other components.
		 * 
		 * @tparam ComponentType Deduced from component.
		 * @param component The component to register.
		 */
		template<typename ComponentType>
		void component(ComponentType& component)
		{
			setter<ComponentType>::set(m_components, component);
		}

		/**
		 * @brief Call the subscribed notification callbacks for Topic.
		 * 
		 * @tparam Topic The topic.
		 */
		template<typename Topic>
		void notify()
		{
			dispatcher<>::template notify<Topic>(m_components);
		}

		/**
		 * @brief Call the subscribed notification and publication
		 * callbacks for Topic.
		 * 
		 * @tparam Topic The topic.
		 * @tparam Args Deduced from args.
		 * @param args Const ref arguments to the callbacks.
		 */
		template<typename Topic, typename... Args>
		void publish(const Args&... args)
		{
			dispatcher<>::template publish<Topic>(m_components, args...);
		}
		
		/**
		 * @brief Call the service callback from Topic's provider.
		 * 
		 * @tparam Topic The topic.
		 * @tparam Args Deduced from args.
		 * @param args Arguments forwarded to the callback.
		 * @return Return<Topic> The possibly void return value.
		 */
		template<typename Topic, typename... Args>
		Return<Topic> call(Args&&... args)
		{
			return caller<Topic>::call(m_components, std::forward<Args>(args)...);
		}

	private:
		/**
		 * @brief Helper struct to scan all components and call
		 * their notification and publication callback.
		 * 
		 * @tparam ComponentIndex=0 Use default.
		 * @tparam bool=true Use default.
		 */
		template<std::size_t ComponentIndex=0, bool=true>
		struct dispatcher
		{
			template<typename TopicType, typename... Args>
			static void publish(ComponentPtrs& components, const Args&... args)
			{
				assert(std::get<ComponentIndex>(components) != nullptr);
				Topic<TopicType>::onNotify(*std::get<ComponentIndex>(components));
				Topic<TopicType>::onPublish(*std::get<ComponentIndex>(components), args...);
				dispatcher<ComponentIndex+1, true>::template publish<TopicType>(components, args...);
			}
			template<typename TopicType>
			static void notify(ComponentPtrs& components)
			{
				assert(std::get<ComponentIndex>(components) != nullptr);
				Topic<TopicType>::onNotify(*std::get<ComponentIndex>(components));
				dispatcher<ComponentIndex+1, true>::template notify<TopicType>(components);
			}
		};
		template<bool dummy>
		struct dispatcher<sizeof...(ComponentTypes), dummy>
		{
			template<typename TopicType, typename... Args>
			static void publish(ComponentPtrs&, const Args&...)
			{}
			template<typename TopicType>
			static void notify(ComponentPtrs&)
			{}
		};

		/**
		 * @brief Helper struct to find a service's provider and call its
		 * callback.
		 * 
		 * @tparam TopicType The service topic.
		 * @tparam ComponentIndex=0 Use default.
		 * @tparam Provider=Provider<TopicType> Use default.
		 */
		template<typename TopicType, std::size_t ComponentIndex=0, typename Provider=Provider<TopicType>>
		struct caller
		{
			template<typename... Args>
			static Return<TopicType> call(ComponentPtrs& components, Args&&... args)
			{
				return caller<TopicType, ComponentIndex+1>::call(components, std::forward<Args>(args)...);
			}
		};
		template<typename TopicType, std::size_t ComponentIndex>
		struct caller<TopicType, ComponentIndex, typename std::tuple_element<ComponentIndex, Components>::type>
		{
			template<typename... Args>
			static Return<TopicType> call(ComponentPtrs& components, Args&&... args)
			{
				return Topic<TopicType>::onCall(*std::get<ComponentIndex>(components), std::forward<Args>(args)...);
			}
		};

		/**
		 * @brief Helper struct to find a component and set its pointer.
		 * 
		 * @tparam ComponentType The component.
		 * @tparam ComponentIndex=0 Use default.
		 */
		template<typename ComponentType, std::size_t ComponentIndex=0>
		struct setter
		{
			template<typename... Args>
			static void set(ComponentPtrs& components, ComponentType& component)
			{
				return setter<ComponentType, ComponentIndex+1>::set(components, component);
			}
		};
		template<std::size_t ComponentIndex>
		struct setter<typename std::tuple_element<ComponentIndex, Components>::type, ComponentIndex>
		{
			static void set(ComponentPtrs& components, typename std::tuple_element<ComponentIndex, Components>::type& component)
			{
				std::get<ComponentIndex>(components) = &component;
			}
		};

		/// Pointers to the components
		ComponentPtrs m_components;
	};
}
