// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "contains.hpp"
#include "types.hpp"

namespace set
{
namespace details
{
// Base template.
template <Types lhs, typename... rhs_types> struct concatenate_if_not_contained;
// Specialization for single set::types: return it.
template <Types lhs> struct concatenate_if_not_contained<lhs>
{
    using type = lhs;
};
// Specialization for first_rhs_type contained in lhs_types: do not concatenate and recurse.
template <typename... lhs_types, typename first_rhs_type, typename... other_rhs_types>
struct concatenate_if_not_contained<set::types<lhs_types...>, first_rhs_type, other_rhs_types...>
{
    using type = typename concatenate_if_not_contained<set::types<lhs_types...>, other_rhs_types...>::type;
};
// Specialization for first_rhs_type not contained in lhs_types: do concatenate and recurse.
template <typename... first_types, typename first_rhs_type, typename... other_rhs_types>
requires(!contains<first_rhs_type, first_types...>) struct concatenate_if_not_contained<
    set::types<first_types...>, first_rhs_type, other_rhs_types...>
{
    using type =
        typename concatenate_if_not_contained<set::types<first_types..., first_rhs_type>, other_rhs_types...>::type;
};

// Base template.
template <Types... type_sets> struct disjunction;
// Specialization for single set::types: return it.
template <Types type_set> struct disjunction<type_set>
{
    using type = type_set;
};
// Specialization for at least two set::types: computes the disjunction of the first two and recurses.
template <typename... first_types, typename... second_types, Types... other_type_sets>
struct disjunction<set::types<first_types...>, set::types<second_types...>, other_type_sets...>
{
    // Disjunction of the first two set::types
    using first_and_second = typename concatenate_if_not_contained<set::types<first_types...>, second_types...>::type;

    // Return first_and_second if no other argument, else recurse.
    using type = typename disjunction<first_and_second, other_type_sets...>::type;
};
} // namespace details

template <Types... type_sets> using disjunction = typename details::disjunction<type_sets...>::type;
} // namespace set