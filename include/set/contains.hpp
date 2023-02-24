// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "indexes.hpp"
#include "types.hpp"

#include <algorithm>

namespace indexes
{
template <std::size_t N, std::size_t M> constexpr bool contains(indexes::array<N> containing, array<M> contained)
{
    return std::search(containing.cbegin(), containing.cend(), contained.cbegin(), contained.cend()) !=
           containing.cend();
}
} // namespace indexes

namespace types
{
// Base template, fails to compile if no specialization is triggered.
template <typename...> constexpr struct fail contains;

// The silly constructs below using template template parameters and a requires clause is used to access the parameters
// of the template template parameter while constraining it to the Set concept.

// single contained type with containing type_set.
template <typename contained_type, template <typename...> typename containing, typename... containing_types>
requires(Set<containing<containing_types...>>)
constexpr bool contains<containing<containing_types...>, contained_type> =
    (std::is_same_v<contained_type, containing_types> || ...);

// contained type_set with containing type_set
template <template <typename...> typename contained, typename... contained_types, Set containing>
requires(Set<contained<contained_types...>>)
constexpr bool contains<containing, contained<contained_types...>> = (contains<containing, contained_types> && ...);
} // namespace types
