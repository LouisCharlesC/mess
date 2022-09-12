// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sequences.hpp"

#include <cstdint>

namespace mess
{
namespace details
{
template <std::size_t index, std::size_t... containing_indexes> consteval bool contains()
{
    return ((index == containing_indexes) || ...);
}

template <std::size_t... contained_indexes, std::size_t... containing_indexes>
consteval bool contains(indexes<contained_indexes...>, indexes<containing_indexes...>)
{
    return (contains<contained_indexes, containing_indexes...>() && ...);
}
} // namespace details

template <typename contained_indexes, typename containing_indexes>
constexpr bool contains = details::contains(contained_indexes(), containing_indexes());
} // namespace mess
