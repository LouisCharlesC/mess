// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "contains.hpp"
#include "indexes.hpp"
#include "types.hpp"

#include <cstddef>
#include <tuple>

namespace types
{
namespace details
{
// A type used to store an index, parameterized by a type.
template <typename> struct type_index
{
    constexpr type_index(std::size_t index) : index(index){};
    std::size_t index;
};

// A tuple of type_index, one type_index for each type in type_set_ with its index set to the type's index in
// type_set_.
template <Set type_set_, typename = std::make_index_sequence<size<type_set_>>> constexpr int index_map;
template <Set type_set_, std::size_t... indexes>
constexpr auto index_map<type_set_, std::index_sequence<indexes...>> =
    std::tuple<type_index<at<indexes, type_set_>>...>(indexes...);
} // namespace details

// Get the index stored in the type_index<type_> from index_map<type_set_>.
template <Set type_set_, typename type_>
requires(contains<type_set_, type_>)
constexpr std::size_t index_of = std::get<details::type_index<type_>>(details::index_map<type_set_>).index;

// Base template, fails to compile if the specialization is not triggered.
template <Set all_types, Set subset_of_types>
requires(contains<all_types, subset_of_types>)
constexpr struct fail to_indexes;

// Specialization to get the types pack from subset_of_types.
template <Set all_types, typename... subset_of_types>
constexpr auto to_indexes<all_types, set<subset_of_types...>> = indexes::set<index_of<all_types, subset_of_types>...>;
} // namespace types
