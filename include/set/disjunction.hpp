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
template <TypeSet lhs_set, typename... rhs_types> struct concatenate_if_not_contained;
// Specialization for type_set only: return it.
template <TypeSet lhs_set> struct concatenate_if_not_contained<lhs_set>
{
    using type = lhs_set;
};
// Specialization for first_rhs_type contained in lhs_types: do not concatenate and recurse.
template <typename... lhs_types, typename first_rhs_type, typename... other_rhs_types>
struct concatenate_if_not_contained<type_set<lhs_types...>, first_rhs_type, other_rhs_types...>
{
    using not_concatenated = type_set<lhs_types...>;
    using type = typename concatenate_if_not_contained<not_concatenated, other_rhs_types...>::type;
};
// Specialization for first_rhs_type not contained in lhs_types: do concatenate and recurse.
template <typename... lhs_types, typename first_rhs_type, typename... other_rhs_types>
requires(!contains<first_rhs_type, lhs_types...>) struct concatenate_if_not_contained<
    type_set<lhs_types...>, first_rhs_type, other_rhs_types...>
{
    using concatenated = type_set<lhs_types..., first_rhs_type>;
    using type = typename concatenate_if_not_contained<concatenated, other_rhs_types...>::type;
};

// Base template.
template <TypeSet... type_sets> struct recursive_disjunction;
// Specialization for no type_set: return an empty type_set.
template <> struct recursive_disjunction<>
{
    using type = type_set<>;
};
// Specialization for single type_set: return it.
template <TypeSet type_set_> struct recursive_disjunction<type_set_>
{
    using type = type_set_;
};
// Specialization for at least two type_set: computes the disjunction of the first two and recurses.
template <typename... first_types, typename... second_types, TypeSet... other_type_sets>
struct recursive_disjunction<type_set<first_types...>, type_set<second_types...>, other_type_sets...>
{
    // Disjunction of the first two type_set
    using disjunction_of_first_two_sets =
        typename concatenate_if_not_contained<type_set<first_types...>, second_types...>::type;

    // Return disjunction_of_first_two_sets if no other argument, else recurse.
    using type = typename recursive_disjunction<disjunction_of_first_two_sets, other_type_sets...>::type;
};
} // namespace details

template <TypeSet... type_sets> using disjunction = typename details::recursive_disjunction<type_sets...>::type;
} // namespace set