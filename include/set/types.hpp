// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <tuple>
#include <type_traits>

namespace set
{
namespace details
{
template <typename unique, typename... Ts>
constexpr bool is_unique = (static_cast<std::size_t>(std::is_same_v<unique, Ts>) + ...) == 1;
template <typename... Ts> constexpr bool all_unique = (is_unique<Ts, Ts...> && ...);
} // namespace details

template <typename... Ts>
requires details::all_unique<Ts...>
using types = std::tuple<Ts...>;

template <typename type> constexpr bool is_types = false;
template <typename... Ts> constexpr bool is_types<set::types<Ts...>> = true;

template <typename T>
concept Types = is_types<T>;

template <std::size_t index> using index_constant = std::integral_constant<std::size_t, index>;
template <std::size_t... Is> using indexes = set::types<index_constant<Is>...>;
} // namespace set