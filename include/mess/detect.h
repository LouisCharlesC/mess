/**
 * @file detect.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-09-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <type_traits>
#include <utility>

namespace mess
{
	namespace detect
	{
		template<typename...>
		using void_t = void;

		template<typename Component, typename=void>
		struct Core
		{
			using type = Component&;
		};
		template<typename Component>
		struct Core<Component, void_t<typename Component::Core>>
		{
			using type = typename Component::Core&;
		};

		// enum class EndpointType
		// {
		// 	Non,
		// 	Receive,
		// 	Consume
		// };

		struct NotAnEndpoint {};
		struct VoidEndpoint {};
		struct ReturnEndpoint {};
		struct ReceiveEndpoint {};
		struct ConsumeEndpoint {};
		struct VoidReceiveEndpoint: public VoidEndpoint, public ReceiveEndpoint {};
		struct ReturnReceiveEndpoint: public ReturnEndpoint, public ReceiveEndpoint {};
		struct VoidConsumeEndpoint: public VoidEndpoint, public ConsumeEndpoint {};
		struct ReturnConsumeEndpoint: public ReturnEndpoint, public ConsumeEndpoint {};
		template<typename Return>
		struct ReceiveEndpointHelper: ReturnReceiveEndpoint {};
		template<>
		struct ReceiveEndpointHelper<void>: VoidReceiveEndpoint {};
		template<typename Return>
		struct ConsumeEndpointHelper: ReturnConsumeEndpoint {};
		template<>
		struct ConsumeEndpointHelper<void>: VoidConsumeEndpoint {};

		namespace impl
		{
			template<std::size_t Index, typename Tag, typename Hub, typename, typename... Payload>
			struct TypeOfEndpoint: NotAnEndpoint
			{};
			template<std::size_t Index, typename Tag, typename Hub, typename... Payload>
			struct TypeOfEndpoint
				< Index,
					Tag,
					Hub,
					void_t<decltype(Hub::template Component<Index>::receive(
						std::declval<Tag>(),
						std::declval<Hub&>(),
						std::declval<typename Hub::template Core<Index>>(),
						std::declval<Payload>()...))>,
					Payload...>
				: ReceiveEndpointHelper<decltype(Hub::template Component<Index>::receive(std::declval<Tag>(), std::declval<Hub&>(), std::declval<typename Hub::template Core<Index>>(), std::declval<Payload>()...))>
			{};
			template<std::size_t Index, typename Tag, typename Hub, typename... Payload>
			struct TypeOfEndpoint
				< Index,
					Tag,
					Hub,
					void_t<decltype(Hub::template Component<Index>::consume(
						std::declval<Tag>(),
						std::declval<Hub&>(),
						std::declval<typename Hub::template Core<Index>>(),
						std::declval<Payload>()...))>,
					Payload...>
				: ConsumeEndpointHelper<decltype(Hub::template Component<Index>::consume(std::declval<Tag>(), std::declval<Hub&>(), std::declval<typename Hub::template Core<Index>>(), std::declval<Payload>()...))>
			{};
		}
		template<std::size_t Index, typename Tag, typename Hub, typename... Payload>
		using TypeOfEndpoint = impl::TypeOfEndpoint<Index, Tag, Hub, void, Payload...>;

		template<std::size_t Index, typename Tag, typename Hub, typename... Payload>
		static constexpr bool IsAnEndpoint()
		{
			return !std::is_base_of<NotAnEndpoint, TypeOfEndpoint<Index, Tag, Hub, Payload...>>::value;
		}
		template<std::size_t Index, typename Tag, typename Hub, typename... Payload>
		static constexpr bool IsAVoidEndpoint()
		{
			return std::is_base_of<VoidEndpoint, TypeOfEndpoint<Index, Tag, Hub, Payload...>>::value;
		}
		template<std::size_t Index, typename Tag, typename Hub, typename... Payload>
		static constexpr bool IsAReturnEndpoint()
		{
			return std::is_base_of<ReturnEndpoint, TypeOfEndpoint<Index, Tag, Hub, Payload...>>::value;
		}
		template<std::size_t Index, typename Tag, typename Hub, typename... Payload>
		static constexpr bool IsAReceiveEndpoint()
		{
			return std::is_base_of<ReceiveEndpoint, TypeOfEndpoint<Index, Tag, Hub, Payload...>>::value;
		}
		template<std::size_t Index, typename Tag, typename Hub, typename... Payload>
		static constexpr bool IsAConsumeEndpoint()
		{
			return std::is_base_of<ConsumeEndpoint, TypeOfEndpoint<Index, Tag, Hub, Payload...>>::value;
		}
		template<std::size_t Index, typename Tag, typename Hub, typename... Payload>
		static constexpr bool IsAVoidReceiveEndpoint()
		{
			return std::is_base_of<VoidReceiveEndpoint, TypeOfEndpoint<Index, Tag, Hub, Payload...>>::value;
		}
		template<std::size_t Index, typename Tag, typename Hub, typename... Payload>
		static constexpr bool IsAVoidConsumeEndpoint()
		{
			return std::is_base_of<VoidConsumeEndpoint, TypeOfEndpoint<Index, Tag, Hub, Payload...>>::value;
		}
		template<std::size_t Index, typename Tag, typename Hub, typename... Payload>
		static constexpr bool IsReturnReceiveEndpoint()
		{
			return std::is_base_of<ReturnReceiveEndpoint, TypeOfEndpoint<Index, Tag, Hub, Payload...>>::value;
		}
		template<std::size_t Index, typename Tag, typename Hub, typename... Payload>
		static constexpr bool IsAReturnConsumeEndpoint()
		{
			return std::is_base_of<ReturnConsumeEndpoint, TypeOfEndpoint<Index, Tag, Hub, Payload...>>::value;
		}
	}
}