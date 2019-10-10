/**
 * @file dispatch.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-10-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "detect.h"

#include <tuple>
#include <type_traits>

namespace mess
{
	namespace dispatch
	{
		struct None {};

		template<typename Tag>
		struct To
		{
			template<typename Hub, typename... Payload>
			struct With
			{
				struct NoConsumeYet
				{
					template<std::size_t... ReturnReceiveIndexes>
					struct SeenSoFar
					{
						static auto proceedWith(std::integral_constant<std::size_t, Hub::NbrOfComponents>, const Hub& hub, Payload&&... message)
						{
							return std::make_tuple(std::tuple_element<ReturnReceiveIndexes, typename Hub::ComponentsTuple>::type::receive(Tag(), hub, std::get<ReturnReceiveIndexes>(hub.m_cores), std::forward<const Payload>(message)...)...);
						}
						template<std::size_t CurrentIndex>
						static auto proceedWith(std::integral_constant<std::size_t, CurrentIndex>, const Hub& hub, Payload&&... message)
						{
							return doProceedWith(std::integral_constant<std::size_t, CurrentIndex>(), detect::TypeOfEndpoint<CurrentIndex, Tag, Hub, Payload...>(), hub, std::forward<Payload>(message)...);
						}
						template<std::size_t CurrentIndex>
						static auto doProceedWith(std::integral_constant<std::size_t, CurrentIndex>, detect::NotAnEndpoint, const Hub& hub, Payload&&... message)
						{
							return proceedWith(std::integral_constant<std::size_t, CurrentIndex+1>(), hub, std::forward<Payload>(message)...);
						}
						template<std::size_t CurrentIndex>
						static auto doProceedWith(std::integral_constant<std::size_t, CurrentIndex>, detect::VoidReceiveEndpoint, const Hub& hub, Payload&&... message)
						{
							Hub::template Component<CurrentIndex>::receive(Tag(), hub, std::get<CurrentIndex>(hub.m_cores), std::forward<const Payload>(message)...);
							return proceedWith(std::integral_constant<std::size_t, CurrentIndex+1>(), hub, std::forward<Payload>(message)...);
						}


						// template<typename CurrentEndpoint>
						// static auto process(detect::ReturnReceiveEndpoint, const Hub& hub, Payload&&... message) -> void
						// {
						// 	SeenSoFar<CurrentEndpoint, ReturnReceiveEndpoints...>::YetToScan<OtherEndpoints...>::process<NextEndpoint>(detect::TypeOfEndpoint<NextEndpoint, Tag, Hub, Payload...>(), hub, std::forward<Payload>(message)...);
						// }
						// template<typename CurrentEndpoint>
						// static auto process(detect::ConsumeEndpoint, const Hub& hub, Payload&&... message) -> void
						// {
						// 	Found<CurrentEndpoint>::SeenSoFar<ReturnReceiveEndpoints...>::YetToScan<OtherEndpoints...>::process<NextEndpoint>(detect::TypeOfEndpoint<NextEndpoint, Tag, Hub, Payload...>(), hub, std::forward<Payload>(message)...);
						// }
						
					};
				};
				// template<typename ConsumeEndpoint>
				// struct Found
				// {
				// 	template<typename... ReturnReceiveEndpoints>
				// 	struct ReturnReceiveSeenSoFar
				// 	{
				// 		template<typename... ReturnConsumeEndpoints>
				// 		struct ReturnConsumeSeenSoFar
				// 		{
				// 		template<typename NextEndpoint, typename... OtherEndpoints>
				// 		struct YetToScan
				// 		{
				// 			template<typename CurrentEndpoint>
				// 			static auto Current(NotAnEndpoint, const Hub& hub, Payload&&... message) -> void
				// 			{
				// 				YetToScan<OtherEndpoints>::Current<NextEndpoint>(detect::TypeOfEndpoint<NextEndpoint>(), std::forward<Payload>(message)...);
				// 			}
				// 			template<typename CurrentEndpoint>
				// 			static auto Current(VoidReceiveEndpoint, const Hub& hub, Payload&&... message) -> void
				// 			{
				// 				CurrentEndpoint::receive(Tag(), hub, hub.getCore<CurrentEndpoint>(), std::forward<const Payload>(message)...);
				// 				YetToScan<OtherEndpoints>::Current<NextEndpoint>(detect::TypeOfEndpoint<NextEndpoint>(), std::forward<Payload>(message)...);
				// 			}
				// 			template<typename CurrentEndpoint>
				// 			static auto Current(ReturnReceiveEndpoint, const Hub& hub, Payload&&... message) -> void
				// 			{
				// 				SeenSoFar<CurrentEndpoint, ReturnEndpoints...>::YetToScan<OtherEndpoints>::Current<NextEndpoint>(detect::TypeOfEndpoint<NextEndpoint>(), std::forward<Payload>(message)...);
				// 			}
				// 			template<typename CurrentEndpoint>
				// 			static auto Current(ConsumeEndpoint, const Hub& hub, Payload&&... message) -> void
				// 			{
				// 				SeenSoFar<CurrentEndpoint, ReturnEndpoints...>::YetToScan<OtherEndpoints>::Current<NextEndpoint>(detect::TypeOfEndpoint<NextEndpoint>(), std::forward<Payload>(message)...);
				// 			}
				// 		};
				// 		template<>
				// 		struct YetToScan<None>
				// 		{
				// 			static auto Current(NotAnEndpoint, const Hub& hub, Payload&&... message) -> void
				// 			{
				// 				return std::make_tuple(ReceiveEndpoints::receive(Tag(), hub, hub.getCore<ReceiveEndpoints>(), std::forward<const Payload>(message)...)...,
				// 															 ConsumeEndpoints::consume(Tag(), hub, hub.getCore<ReceiveEndpoints>(), std::forward<const Payload>(message)...)...,);
				// 			}
				// 		};
				// 	};
				// };

				static void start(const Hub& hub, Payload&&... message)
				{
					NoConsumeYet::template SeenSoFar<>::template proceedWith(std::integral_constant<std::size_t, 0>(), hub, std::forward<Payload>(message)...);
				}
			};
		};
	}
}