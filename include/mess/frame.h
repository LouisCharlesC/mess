/**
 * @file frame.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include <tuple>

namespace mess
{
	template<typename TypesTuple, typename... Names>
	class Frame
	{
		template<typename P, std::size_t... Indexes>
		static constexpr std::size_t getIndex(std::index_sequence<Indexes...>)
		{
			using NamesTuple = std::tuple<Names...>;
			return ((static_cast<std::size_t>(std::is_same<std::tuple_element_t<Indexes, NamesTuple>, P>::value)*Indexes) + ... + 0);
		}

	public:
		template<typename... Types>
		constexpr Frame(Types&&... values):
			m_values(std::forward<Types>(values)...)
		{}
		constexpr Frame(TypesTuple&& values):
			m_values(std::move(values))
		{}

		template<typename NameToSearch>
		static constexpr bool contains()
		{
			constexpr std::size_t foundNTimes = (static_cast<std::size_t>(std::is_same<NameToSearch, Names>::value) + ... + 0);
			static_assert(foundNTimes <= 1, "Names... must be pack of unique types");
			return foundNTimes == 1;
		}

		template<typename NameToGet>
		constexpr auto get()
		{
			static_assert(Frame::contains<NameToGet>(), "Check that NameToGet is contained in Names... with function Frame::constains<NameToGet>() before clling Frame::get<NameToGet>()");
			return std::get<Frame::getIndex<NameToGet>(std::index_sequence_for<Names...>())>(m_values);
		}

		template<typename Name, typename Type>
		constexpr auto extend(Type&& value)
		{
			using ExtendedTypesTuple = decltype(std::tuple_cat(m_values, std::make_tuple(std::forward<Type>(value))));
			return ExtendedTypesTuple(std::tuple_cat(m_values, std::make_tuple(std::forward<Type>(value))));
		}

	private:
		TypesTuple m_values;
	};

	template<typename... Names, typename... Types>
	constexpr Frame<std::tuple<Types...>, Names...> make_frame(Types&&... values)
	{
		return Frame<std::tuple<Types...>, Names...>(std::forward<Types>(values)...);
	}
} // namespace mess