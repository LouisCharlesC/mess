// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "types.hpp"

namespace set
{
// single contained type with containing types pack.
template <typename contained_type, typename... containing_types>
static constexpr bool contains = (std::is_same_v<contained_type, containing_types> || ...);

// single contained type with containing type_set.
template <typename contained_type, typename... containing_types>
static constexpr bool contains<contained_type, types<containing_types...>> =
    contains<contained_type, containing_types...>;

// contained type_set with containing types pack
template <typename... contained_types, typename... containing_types>
static constexpr bool contains<types<contained_types...>, containing_types...> =
    (contains<contained_types, containing_types...> && ...);

// contained type_set with containing type_set
template <typename... contained_types, typename... containing_types>
static constexpr bool contains<types<contained_types...>, types<containing_types...>> =
    (contains<contained_types, containing_types...> && ...);
} // namespace set
