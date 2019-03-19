/**
 * @file indicestrick.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-01
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <cstdint>
#include <vector>

namespace tricks
{
	struct dummy_t {constexpr operator bool() {return true;}};
	static constexpr dummy_t dummy;

	template <std::size_t... Indices>
	struct indices {};

	template<typename... FullSetElements>
	struct FullSet
	{
		template <typename CurrentSetFirstElement, typename... CurrentSetOtherElements>
		struct CurrentSet
		{
			template<bool dummy, typename... SubsetElements>
			struct SubSet
			{
				template <std::size_t CurrentSetFirstElementIndex, std::size_t... SubsetIndices>
				struct MakeIndices: CurrentSet<CurrentSetOtherElements...>::template SubSet<dummy, SubsetElements...>::template MakeIndices<CurrentSetFirstElementIndex+1, SubsetIndices...>
				{
					static constexpr std::size_t index = CurrentSet<CurrentSetOtherElements...>::template SubSet<dummy, SubsetElements...>::template MakeIndices<CurrentSetFirstElementIndex+1, SubsetIndices...>::index;
				};
			};

			template<bool dummy, typename SubsetSecondElement, typename... SubsetOtherElements>
			struct SubSet<dummy, CurrentSetFirstElement, SubsetSecondElement, SubsetOtherElements...>
			{
				template <std::size_t CurrentSetFirstElementIndex, std::size_t... SubsetIndices>
				struct MakeIndices: CurrentSet<FullSetElements...>::template SubSet<dummy, SubsetSecondElement, SubsetOtherElements...>::template MakeIndices<0, SubsetIndices..., CurrentSetFirstElementIndex>
				{};
			};

			template<bool dummy>
			struct SubSet<dummy, CurrentSetFirstElement>
			{
				template <std::size_t CurrentSetFirstElementIndex, std::size_t... SubsetIndices>
				struct MakeIndices: indices<SubsetIndices..., CurrentSetFirstElementIndex>
				{
					static constexpr std::size_t index = CurrentSetFirstElementIndex;
				};
			};
		};

		template<typename... SubsetElements>
		struct MakeIndices: CurrentSet<FullSetElements...>::template SubSet<dummy, SubsetElements...>::template MakeIndices<0>
		{
			static constexpr std::size_t index = CurrentSet<FullSetElements...>::template SubSet<dummy, SubsetElements...>::template MakeIndices<0>::index;
		};
	};

}  // namespace tricks