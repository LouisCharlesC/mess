// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "concatenate.hpp"
#include "sequences.hpp"

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace mess
{
template <typename flat_graph, std::size_t index>
constexpr bool is_leaf = std::tuple_element_t<index, flat_graph>::successor_tags::size == 0;

template <typename flat_graph, std::size_t... indexes_v>
using leaf_nodes = concatenate<std::conditional_t<is_leaf<flat_graph, indexes_v>, indexes<indexes_v>, indexes<>>...>;
} // namespace mess