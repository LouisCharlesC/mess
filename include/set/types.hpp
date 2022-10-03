// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <tuple>
#include <type_traits>

namespace set
{
template <typename unique, typename... types>
constexpr bool is_unique = (static_cast<std::size_t>(std::is_same_v<unique, types>) + ...) == 1;
template <typename... types> constexpr bool all_unique = (is_unique<types, types...> && ...);

template <typename... Ts>
requires all_unique<Ts...>
using types = std::tuple<Ts...>;

template <typename type> constexpr bool is_type_set = false;
template <typename... types> constexpr bool is_type_set<set::types<types...>> = true;

template <typename T>
concept TypeSet = is_type_set<T>;

template <std::size_t index> using index_constant = std::integral_constant<std::size_t, index>;
template <std::size_t... Is> using indexes = set::types<index_constant<Is>...>;
} // namespace set