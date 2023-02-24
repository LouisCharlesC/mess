// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <tuple>
#include <type_traits>

namespace types
{
template <typename unique, typename... Ts>
constexpr bool is_unique = (static_cast<std::size_t>(std::is_same_v<unique, Ts>) + ...) == 1;

// Base template, returns true if all Ts are unique.
template <typename... Ts> constexpr bool all_unique = (is_unique<Ts, Ts...> && ...);
// Specialization to get the Ts out of template, delegates to the base template.
template <template <typename...> typename T, typename... Ts> constexpr bool all_unique<T<Ts...>> = all_unique<Ts...>;

namespace details
{
// Base template, returns false.
template <typename> constexpr bool is_tuple_like = false;
// Specialization for std::tuple, return true.
template <template <typename...> typename T, typename... Ts> constexpr bool is_tuple_like<T<Ts...>> = true;
} // namespace details

// Concept of a set of unique types.
template <typename T>
concept Set = details::is_tuple_like<T> && all_unique<T>;

// A set of unique types.
template <typename... Ts>
requires Set<std::tuple<Ts...>>
using set = std::tuple<Ts...>;

// Helpers to hide the std::tuple implementation.
template <Set type_set_> constexpr std::size_t size = std::tuple_size_v<type_set_>;
template <std::size_t index, Set type_set_> using at = std::tuple_element_t<index, type_set_>;
} // namespace types