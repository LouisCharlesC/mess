// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <cstddef>
#include <tuple>
#include <utility>

namespace sequence
{
template <std::size_t index> using index_constant = std::integral_constant<std::size_t, index>;

template <typename... types> class type_set;
template <std::size_t... indexes> using index_set = type_set<index_constant<indexes>...>;

template <typename... types> class type_set
{
    template <typename type> struct type_index
    {
        std::size_t index;
    };

  public:
    static constexpr std::size_t size = sizeof...(types);

  private:
    template <std::size_t... indexes> static consteval auto make_type_index_map(std::index_sequence<indexes...>)
    {
        return std::tuple<type_index<types>...>({.index = indexes}...);
    }

    static constexpr auto type_index_map = make_type_index_map(std::make_index_sequence<size>());

  public:
    template <typename... rhs_types> constexpr type_set<types..., rhs_types...> operator+(type_set<rhs_types...>)
    {
        return {};
    }

  public:
    template <typename contained_type> static constexpr bool contains = (std::is_same_v<contained_type, types> || ...);
    template <typename... contained_types>
    static constexpr bool contains<type_set<contained_types...>> = (contains<contained_types> && ...);

    template <typename rhs_set> using concatenate = decltype(std::declval<type_set>() + std::declval<rhs_set>());

    template <typename type>
    requires type_set::contains<type>
    static constexpr std::size_t index_of = std::get<type_index<type>>(type_index_map).index;

    template <std::size_t index>
    requires type_set::contains<index_constant<index>>
    static constexpr std::size_t index_of<index_constant<index>> =
        std::get<type_index<index_constant<index>>>(type_index_map).index;
};

} // namespace sequence