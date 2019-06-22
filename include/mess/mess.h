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
#include <initializer_list>
#include <iterator>
#include <tuple>

#include <iostream>

// TODO: test publish with different subscribers patterns
			// add nested struct in endpoints to facilitate explicit instantiation
			// provide a forwarding endpoint ? (find the right name in eip)

	// Forward declarations
namespace mess
{
	template<typename...> class Broker;
}

namespace mess
{
	/**
	 * @brief Base trait class to define Channels. Specialize this class
	 * for your Channel, and have it inherit from
	 * Subscribers<list of subscribing endpoints>.
	 * 
	 * @tparam ChannelUID A type that identifies the channel.
	 */
	template<typename ChannelUID>
	struct Channel;

	/// A few helper structs for internal use
	namespace
	{
		template<typename ChannelUID>
		using ASubscriber = typename Channel<ChannelUID>::Subscribers::ASubscriber;

		template<typename Endpoint, typename ChannelUID, typename... Args>
		using ReplyEndpoint = decltype(Endpoint::onPublish(ChannelUID(), std::declval<Broker<>&>(), std::declval<typename Endpoint::Component&>(), std::declval<Args>()...));

		template<typename ChannelUID, typename... Args>
		using ReplyChannel = ReplyEndpoint<ASubscriber<ChannelUID>, ChannelUID, Args...>;
	}

	template<typename FirstEndpoint, typename... OtherEndpoints>
	class Subscribers
	{
		/// Friend declaration
		template<typename...> friend class Broker;

		/// One of the subscribers
		using ASubscriber = FirstEndpoint;

		template<typename ChannelUID, typename BrokerType, typename... Args>
		static auto publish(BrokerType& broker, Args&&... args) -> decltype(BrokerType::template RemoveUnknown<FirstEndpoint, OtherEndpoints...>::type::template publish<ChannelUID>(broker, std::forward<Args>(args)...))
		{
			static_assert(std::is_same<std::tuple<ReplyEndpoint<FirstEndpoint, ChannelUID, Args...>, ReplyEndpoint<OtherEndpoints, ChannelUID, Args...>...>, std::tuple<ReplyEndpoint<OtherEndpoints, ChannelUID, Args...>..., ReplyEndpoint<FirstEndpoint, ChannelUID, Args...>>>::value, "All reply types for a channel must be the same");
			
			// Delegate back to the broker
			BrokerType::template RemoveUnknown<FirstEndpoint, OtherEndpoints...>::type::template publish<ChannelUID>(broker, std::forward<Args>(args)...);
		}
	};

	/**
	 * @brief The class through which messages can be published to
	 * Channels.
	 * 
	 * Either provide all Components for the Endpoints at construction,
	 * or use the Builder to provide them one at a time using the set()
	 * function.
	 * 
	 * @tparam Endpoints The Endpoints whose subscription Callbacks the
	 * Broker can call.
	 */
	template<typename... Endpoints>
	class Broker
	{
		/// Friend declaration
		template<typename, typename...> friend class Subscribers;

		/// Number of Endpoints known by this Broker
		static constexpr auto NbrOfEndpoints = sizeof...(Endpoints);
		/// Constant to identify an unknown Endpoint within this Broker
		static constexpr auto UnknownEndpoint = NbrOfEndpoints;

		/// Tuple of Endpoints
		using EndpointsTuple = std::tuple<Endpoints...>;
		/// Tuple to hold references to the Components
		using ComponentRefsTuple = std::tuple<typename Endpoints::Component&...>;

		/**
		 * @brief Helper class to implement C++14's std::get<Type>(tuple)
		 * to get a Component.
		 * Usage example:
		 * std::get<EndpointIndex<Endpoint>::value>(m_components).
		 * 
		 * @tparam Endpoint The Endpoint whose index to return.
		 * @tparam IndexValue=0 For internal use.
		 */
		template<typename Endpoint, std::size_t IndexValue=0>
		struct EndpointIndex
		{
			static constexpr std::size_t value = EndpointIndex<Endpoint, IndexValue+1>::value;
		};
		template<typename Endpoint>
		struct EndpointIndex<Endpoint, NbrOfEndpoints> 
		{
			static constexpr std::size_t value = UnknownEndpoint;
		};
		// Excuse the "IndexValue==NbrOfEndpoints?0:IndexValue" in the tuple_element template arguments. Apparantly, this specialization must be evaluated even though the previous one triggers. This trick ensures that index value passed to tuple_element always is lower than its size.
		template<std::size_t IndexValue>
		struct EndpointIndex<typename std::tuple_element<IndexValue==NbrOfEndpoints?0:IndexValue, EndpointsTuple>::type, IndexValue> 
		{
			static constexpr std::size_t value = IndexValue;
		};

	public:
		/**
		 * @brief Builder for a Broker. Useful to provide the Components one at
		 * a time using the set function rather than all at once at
		 * construction.
		 */
		class Builder
		{
			using ComponentPtrsTuple = std::tuple<typename Endpoints::Component*...>;

		public:
		/**
		 * @brief Sets the Component for an Endpoint.
		 * 
		 * If the Endpoint is not known to the Broker, this call will be
		 * ignored.
		 * 
		 * @tparam endpoint The Endpoint.
		 * @param component The Component.
		 */
			template<typename Endpoint>
			void set(typename Endpoint::Component& component)
			{
				setIfKnown<Endpoint>(IsKnown<Endpoint>(), component);
			}

			/**
			 * @brief Build the Broker.
			 * 
			 * @return Broker The Broker.
			 */
			Broker build() const
			{
				return Broker(*std::get<EndpointIndex<Endpoints>::value>(m_components)...);
			}

		private:
			/// Helper types to determine whether an Endpoint is known or not
			template<typename Endpoint>
			using IsKnown = std::integral_constant<bool, EndpointIndex<Endpoint>::value != Broker::UnknownEndpoint>;
			using Known = std::true_type;
			using Unknown = std::false_type;

			/// Helper functions to set the Component or not
			template<typename Endpoint>
			void setIfKnown(Known, typename Endpoint::Component& component)
			{
				std::get<EndpointIndex<Endpoint>::value>(m_components) = &component;
			}
			template<typename Endpoint>
			void setIfKnown(Unknown, const typename Endpoint::Component&) {}

			/// Pointers to the Components
			ComponentPtrsTuple m_components;
		};

		/**
		 * @brief Construct a new Broker.
		 * 
		 * @param components One Component per Endpoint known by the Broker
		 */
		Broker(typename Endpoints::Component&... components):
			m_components(components...)
		{}

		/**
		 * @brief Publish to a Channel.
		 * 
		 * This will call all subscribing endpoints' callback for this
		 * Channel, forwarding the provided arguments, and returning void or
		 * an std::array of replies.
		 * 
		 * @tparam ChannelUID The Channel to publish to.
		 * @tparam Args Deduced from args.
		 * @param args Data to publish.
		 * @return auto Void or the aggregated replies from the callbacks
		 */
		template<typename ChannelUID, typename... Args>
		auto publish(Args&&... args) -> decltype(Channel<ChannelUID>::Subscribers::template publish<ChannelUID>(*this, std::forward<Args>(args)...))
		{
			/// This is a bit of a mess, bear with me:
			/// This function calls the publish function from Channel<ChannelUID>::Subscribers because this is the class that knowns what Endpoints subscribed to the Channel.
			/// The Channel<ChannelUID>::Subscribers::publish function in turn uses the RemoveUnknown struct from this very Broker to strip the subscribing Endpoints that are unknown to the Broker from the list of Subscribers.
			/// Then, RemoveUnknown::Subscribers::publish is called, which calls the onPublish Callback function of all known subscribing Endpoints.
			/// If the Channel is two-way (callbacks have a return value), they are aggregated into a std::array and returned, otherwise the return is void.
			return Channel<ChannelUID>::Subscribers::template publish<ChannelUID>(*this, std::forward<Args>(args)...);
		}

	private:
		/// Helper struct to keep only the Endpoints known to the Broker from a list of Endpoints
		/// Also, this struct places the last known endpoint at the front of the list of known endpoints, so that this list can easily be split into LastKnownSubscriber and OtherSubscribers... in the KnownSubscribers class
		template<typename Endpoint, typename... OtherEndpoints>
		struct RemoveUnknown
		{
			static constexpr bool IsKnown() {return EndpointIndex<Endpoint>::value != UnknownEndpoint;}

			template<typename LastKnownEnpoint, typename... OtherKnownEndpoints>
			struct Build
			{
				using type = typename std::conditional<IsKnown(), typename RemoveUnknown<OtherEndpoints...>::template Build<Endpoint, OtherKnownEndpoints..., LastKnownEnpoint>::type, typename RemoveUnknown<OtherEndpoints...>::template Build<LastKnownEnpoint, OtherKnownEndpoints...>::type>::type;
			};

			using type = typename std::conditional<IsKnown(), typename RemoveUnknown<OtherEndpoints...>::template Build<Endpoint>::type, typename RemoveUnknown<OtherEndpoints...>::type>::type;
		};
		template<typename Endpoint>
		struct RemoveUnknown<Endpoint>
		{
			static constexpr bool IsKnown() {return EndpointIndex<Endpoint>::value != UnknownEndpoint;}

			template<typename LastKnownEnpoint, typename... OtherKnownEndpoints>
			struct Build
			{
				using type = typename std::conditional<IsKnown(), typename Broker::template KnownSubscribers<typename Broker::template KnownSubscribersBase<Endpoint, OtherKnownEndpoints..., LastKnownEnpoint>>, typename Broker::template KnownSubscribers<typename Broker::template KnownSubscribersBase<LastKnownEnpoint, OtherKnownEndpoints...>>>::type;
			};

			using type = typename std::conditional<IsKnown(), typename Broker::template KnownSubscribers<typename Broker::template KnownSubscribersBase<Endpoint>>, typename Broker::template KnownSubscribers<typename Broker::NoKnownSubscribersBase>>::type;
		};

		/// Constants to represent one-way and two-way channels
		using OneWayChannel = std::true_type;
		using TwoWayChannel = std::false_type;
		template<typename ChannelUID, typename... Args>
		using IsOneWayOrTwoWayChannel = typename std::is_same<void, ReplyChannel<ChannelUID, Args...>>::type;

		template<typename Base>
		struct KnownSubscribers: Base
		{
			/// Helper function to dispatch to one-way and two-way channels
			template<typename ChannelUID, typename... Args>
			static auto publish(Broker& broker, Args&&... args) -> decltype(Base::template publish<ChannelUID>(IsOneWayOrTwoWayChannel<ChannelUID, Args...>(), broker, std::forward<Args>(args)...))
			{
				return Base::template publish<ChannelUID>(IsOneWayOrTwoWayChannel<ChannelUID, Args...>(), broker, std::forward<Args>(args)...);
			}
		};

		/// Helper class to publish to a list of known subscribing Endpoints
		template<typename LastKnownEnpoint, typename... OtherKnownEndpoints>
		class KnownSubscribersBase
		{
		protected:
			/// This function publishes to one-way channels
			template<typename ChannelUID, typename... Args>
			static void publish(OneWayChannel, Broker& broker, Args&&... args)
			{
				// Sorry for this line of code, it is explained here: https://arne-mertz.de/2016/11/more-variadic-templates/
				// I do not know if this form for the call to std::forward: "std::forward<const Args>(args)..." (notice the const) is right, but it produces result I am looking for: arguments are forwarded without ever being moved.
				(void)std::initializer_list<int>{ // gather all the below generated ints in an unused initializer_list
				(OtherKnownEndpoints::onPublish( // call onPublish
					ChannelUID(), // pass the channel tag
					broker, // pass the broker
					std::get<Broker::template EndpointIndex<OtherKnownEndpoints>::value>(broker.m_components), // pass the component
					std::forward<const Args>(args)...) // forward the const-ified arguments, 
				, 0) // tricky: this calls the above onPublish function, but returns 0 rather than void
				..., // do the above for all OtherEndpoints
				(LastKnownEnpoint::onPublish( // do the above for the LastKnownEnpoint, but truly forward the arguments
					ChannelUID(),
					broker,
					std::get<Broker::template EndpointIndex<LastKnownEnpoint>::value>(broker.m_components),
					std::forward<Args>(args)...) // forward the arguments
				, 0)
				};
			}

			/// This function publishes to two-way channels, aggregating the replies into a std::array
			template<typename ChannelUID, typename... Args>
			static std::array<ReplyChannel<ChannelUID, Args...>, sizeof...(OtherKnownEndpoints)+1> publish(TwoWayChannel, Broker& broker, Args&&... args)
			{
				return {
					OtherKnownEndpoints::onPublish(
						ChannelUID(),
						broker,
						std::get<Broker::template EndpointIndex<OtherKnownEndpoints>::value>(broker.m_components),
						std::forward<const Args>(args)...
					)...,
					LastKnownEnpoint::onPublish(
						ChannelUID(),
						broker,
						std::get<Broker::template EndpointIndex<LastKnownEnpoint>::value>(broker.m_components),
						std::forward<Args>(args)...
					)
				};
			}
		};

		/// Result of RemoveUnknown if no Endpoints from the list are known
		class NoKnownSubscribersBase
		{
		protected:
			template<typename ChannelUID, typename... Args>
			static void publish(OneWayChannel, Broker& broker, Args&&... args) {}

			template<typename ChannelUID, typename... Args>
			static std::array<ReplyChannel<ChannelUID, Args...>, 0> publish(TwoWayChannel, Broker& broker, Args&&... ar)
			{
				return {};
			}
		};

		/// Refrence to the components
		const ComponentRefsTuple m_components;
	};
}