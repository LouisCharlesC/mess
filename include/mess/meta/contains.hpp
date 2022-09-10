// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <cstdint>
#include <utility>

namespace mess
{
namespace details
{
template <std::size_t index, std::size_t... super_indexes> consteval bool contains()
{
    return ((index == super_indexes) || ...);
}

template <std::size_t... indexes, std::size_t... super_indexes>
consteval bool contains(std::index_sequence<indexes...>, std::index_sequence<super_indexes...>)
{
    return (contains<indexes, super_indexes...>() && ...);
}
} // namespace details

template <typename sequence, typename super_sequence>
constexpr bool contains = details::contains(sequence(), super_sequence());
} // namespace mess
