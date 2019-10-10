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

#include "detect.h"
#include "dispatch.h"

#include <cstddef>
#include <tuple>
#include <utility>

namespace mess
{
	/**
	 * @brief The class through which messages can be published to
	 * Channels.
	 * 
	 * Either provide all Components for the Components at construction,
	 * or use the Builder to provide them one at a time using the set()
	 * function.
	 * 
	 * @tparam Components The Components whose subscription Callbacks the
	 * Broker can call.
	 */
	template<typename... Components>
	class Hub
	{
	public:

		/// Number of Components known by this Hub
		static constexpr auto NbrOfComponents = sizeof...(Components);

		/// Tuple of Components
		using ComponentsTuple = std::tuple<Components...>;

		/// Tuple to hold references to the Cores
		using CoresTuple = std::tuple<typename detect::Core<Components>::type...>;

		template<std::size_t Index>
		using Component = typename std::tuple_element<Index, ComponentsTuple>::type;
		template<std::size_t Index>
		using Core = typename std::tuple_element<Index, CoresTuple>::type;
		/**
		 * @brief Construct a new Broker.
		 * 
		 * @param components One Component per Component known by the Broker
		 */
		Hub(typename detect::Core<Components>::type&... cores):
			m_cores(cores...)
		{}

		template<typename Tag, typename... Payload>
		auto send(Payload&&... message) const -> void
		{
			dispatch::To<Tag>::template With<Hub, Payload...>::start(*this, std::forward<Payload>(message)...);
		}

		/// Refrence to the components
		const CoresTuple m_cores;
	};
}