// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <numeric>

namespace indexes
{
// The type of a set of indexes.
template <std::size_t N> using array = std::array<std::size_t, N>;

namespace details
{
template <typename> constexpr bool is_array = false;
template <std::size_t N> constexpr bool is_array<array<N>> = true;

template <std::size_t N> constexpr bool all_unique(array<N> indexes)
{
    std::sort(indexes.begin(), indexes.end());
    return std::adjacent_find(indexes.cbegin(), indexes.cend()) == indexes.cend();
}

constexpr auto make_array(auto... Is) -> array<sizeof...(Is)>
{
    return {Is...};
}
} // namespace details

// // Concept of a set of unique indexes.
// template <auto indexes_>
// concept Set = details::is_array<indexes_> && details::all_unique(indexes_);

// A set of unique indexes.
template <std::size_t... Is>
requires(details::all_unique(details::make_array(Is...)))
constexpr auto set = details::make_array(Is...);
} // namespace indexes