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

#include "mess/traits.h"

#include <cassert>
#include <cstddef>
#include <tuple>
#include <type_traits>

namespace mess
{
	template<typename... ComponentTypes>
	class BrokerTemplate
	{
		using Components = std::tuple<ComponentTypes...>;
		using ComponentPtrs = std::tuple<ComponentTypes*...>;

	public:
		BrokerTemplate() = default;

		template<typename Component>
		void component(Component& component)
		{
			setter<Component>::set(m_components, component);
		}

		template<typename TopicType>
		void notify()
		{
			dispatcher<>::template notify<TopicType>(m_components);
		}

		template<typename TopicType, typename... Args>
		void publish(Args&&... args)
		{
			dispatcher<>::template publish<TopicType>(m_components, std::forward<Args>(args)...);
		}
		
		template<typename ServiceType, typename... Args>
		Return<ServiceType> call(Args&&... args)
		{
			return caller<ServiceType>::call(m_components, std::forward<Args>(args)...);
		}

	private:
		template<std::size_t ComponentIndex=0, bool=true>
		struct dispatcher
		{
			template<typename TopicType, typename... Args>
			static void publish(ComponentPtrs& components, Args&&... args)
			{
				assert(std::get<ComponentIndex>(components) != nullptr);
				Topic<TopicType>::onNotify(*std::get<ComponentIndex>(components));
				Topic<TopicType>::onPublish(*std::get<ComponentIndex>(components), std::forward<Args>(args)...);
				dispatcher<ComponentIndex+1, true>::template publish<TopicType>(components, std::forward<Args>(args)...);
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
			static void publish(ComponentPtrs&, Args&&...)
			{}
			template<typename TopicType>
			static void notify(ComponentPtrs&)
			{}
		};

		template<typename ServiceType, std::size_t ComponentIndex=0, typename Provider=Provider<ServiceType>>
		struct caller
		{
			template<typename... Args>
			static Return<ServiceType> call(ComponentPtrs& components, Args&&... args)
			{
				return caller<ServiceType, ComponentIndex+1>::call(components, std::forward<Args>(args)...);
			}
		};
		template<typename ServiceType, std::size_t ComponentIndex>
		struct caller<ServiceType, ComponentIndex, typename std::tuple_element<ComponentIndex, Components>::type>
		{
			template<typename... Args>
			static Return<ServiceType> call(ComponentPtrs& components, Args&&... args)
			{
				return Topic<ServiceType>::onCall(*std::get<ComponentIndex>(components), std::forward<Args>(args)...);
			}
		};

		template<typename Component, std::size_t ComponentIndex=0>
		struct setter
		{
			template<typename... Args>
			static void set(ComponentPtrs& components, Component& component)
			{
				return setter<Component, ComponentIndex+1>::set(components, component);
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

		ComponentPtrs m_components;
	};
}
