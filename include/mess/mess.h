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
#include <iterator>
#include <tuple>
#include <type_traits>

// TODO: make publish secure: prevent moving
			// finish builder
			// add nested struct in components to facilitate explicit instantiation
			// show how to split into files and use expl inst to limit compilation time

namespace mess
{
	/**
	 * @brief Base trait class to define topics.
	 * 
	 * @tparam Topic The topic.
	 */
	template<typename TopicType>
	struct Topic;

	template<typename FirstComponent, typename... OtherComponents>
	struct Subscribe
	{
		using Subscribers = std::tuple<FirstComponent, OtherComponents...>;
	};

	template<typename Component>
	struct Provide
	{
		using Provider = Component;
	};

	template<typename Topic, typename ComponentType, std::size_t Index=0>
	struct IsSubscriber
	{
		static constexpr bool value = IsSubscriber<Topic, ComponentType, Index+1>::value;
		using type = typename std::conditional<value, std::true_type, std::false_type>::type;
	};
	template<typename Topic, std::size_t Index>
	struct IsSubscriber<Topic, typename std::tuple_element<Index==std::tuple_size<typename mess::Topic<Topic>::Subscribers>::value?0:Index, typename mess::Topic<Topic>::Subscribers>::type, Index>
	{
		static constexpr bool value = true;
		using type = typename std::conditional<value, std::true_type, std::false_type>::type;
	};
	template<typename Topic, typename ComponentType>
	struct IsSubscriber<Topic, ComponentType, std::tuple_size<typename mess::Topic<Topic>::Subscribers>::value>
	{
		static constexpr bool value = false;
		using type = typename std::conditional<value, std::true_type, std::false_type>::type;
	};

	/**
	 * @brief The class that connects the components together.
	 * Register the components using the component() function. Then,
	 * notify() to notification topics, publish() to publication topics
	 * and call() service topics.
	 * 
	 * @tparam CoreTypes The components.
	 */
	template<typename... Components>
	class Broker
	{
		using ComponentsTuple = std::tuple<Components...>;
		using CoreRefs = std::tuple<typename Components::Core&...>;

		template<typename ComponentType, std::size_t Index=0>
		struct ComponentIndex
		{
			static constexpr std::size_t value = ComponentIndex<ComponentType, Index+1>::value;
		};
		template<std::size_t Index>
		struct ComponentIndex<typename std::tuple_element<Index, ComponentsTuple>::type, Index> 
		{
			static constexpr std::size_t value = Index;
		};

		/**
		 * @brief Helper struct to scan all components and call
		 * their notification and publication callback.
		 * 
		 * @tparam CoreIndex=0 Use default.
		 * @tparam bool=true Use default.
		 */
		template<typename Topic, std::size_t Index=0>
		struct publisher
		{
			template<typename... Args>
			static void publish(Broker& broker, Args&&... args)
			{
				publish(typename IsSubscriber<Topic, typename std::tuple_element<Index, ComponentsTuple>::type>::type(), broker, std::forward<Args>(args)...);
				publisher<Topic, Index+1>::publish(broker, std::forward<Args>(args)...);
			}

		private:
			template<typename... Args>
			static void publish(std::false_type, Broker& broker, Args&&... args) {}
			template<typename... Args>
			static void publish(std::true_type, Broker& broker, Args&&... args)
			{
				std::tuple_element<Index, ComponentsTuple>::type::onPublish(Topic(), broker, std::get<Index>(broker.m_cores), std::forward<Args>(args)...);
			}
		};
		template<typename Topic>
		struct publisher<Topic, sizeof...(Components)>
		{
			template<typename... Args>
			static void publish(Broker&, Args&&...)
			{}
		};

		template<typename Topic, std::size_t Index=0>
		struct caller
		{
			template<typename... Args>
			static auto call(Broker& broker, Args&&... args) -> decltype(caller<Topic, Index+1>::call(broker, std::forward<Args>(args)...))
			{
				return caller<Topic, Index+1>::call(broker, std::forward<Args>(args)...);
			}
		};
		template<typename Topic>
		struct caller<Topic, ComponentIndex<typename mess::Topic<Topic>::Provider>::value>
		{
			template<typename... Args>
			static auto call(Broker& broker, Args&&... args) -> decltype(mess::Topic<Topic>::template onCall<Topic>(broker, std::get<ComponentIndex<typename Provide<Topic>::Component>::value>(broker.m_cores), std::forward<Args>(args)...))
			{
				return mess::Topic<Topic>::template onCall<Topic>(broker, std::get<ComponentIndex<typename mess::Topic<Topic>::Provider>::value>(broker.m_cores), std::forward<Args>(args)...);
			}
		};

	public:
		class Builder
		{
			using CorePtrs = std::tuple<typename Components::Core*...>;

			Builder() = default;

		/**
		 * @brief This registers a core for a component..
		 * 
		 * @tparam ComponentType The component for which the core will be
		 * registered.
		 * @param component The core to register.
		 */
			template<typename ComponentType>
			void set(typename ComponentType::Core& core)
			{
				setter<ComponentType>::set(m_cores, core);
			}

			Broker build() const
			{
				// return Broker(*std::get<Indexes...>(m_cores)...);
				return Broker();
			}

		private:
		/**
		 * @brief Helper struct to find a component and set its pointer.
		 * 
		 * @tparam ComponentType The component.
		 * @tparam CoreIndex=0 Use default.
		 */
		template<typename ComponentType, std::size_t Index=0>
		struct setter
		{
			static void set(CorePtrs& cores, typename ComponentType::Core& core)
			{
				return setter<ComponentType, Index+1>::set(cores, core);
			}
		};
		template<std::size_t Index>
		struct setter<typename std::iterator_traits<typename std::tuple_element<Index, CorePtrs>::type>::value_type, Index>
		{
			static void set(CorePtrs& cores, typename std::iterator_traits<typename std::tuple_element<Index, CorePtrs>::type>::value_type::Core& core)
			{
				std::get<Index>(cores) = &core;
			}
		};

			CorePtrs m_cores;
		};

		Broker(typename Components::Core&... cores):
			m_cores(cores...)
		{}

		/**
		 * @brief Call the subscribed notification and publication
		 * callbacks for Topic.
		 * 
		 * @tparam Topic The topic.
		 * @tparam Args Deduced from args.
		 * @param args Const ref arguments to the callbacks.
		 */
		template<typename Topic, typename... Args>
		void publish(Args&&... args)
		{
			publisher<Topic>::publish(*this, std::forward<Args>(args)...);
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
		auto call(Args&&... args) -> decltype(caller<Topic>::call(*this, std::forward<Args>(args)...))
		{
			return caller<Topic>::call(*this, std::forward<Args>(args)...);
		}

	private:
		/// Pointers to the cores
		CoreRefs m_cores;
	};
}
