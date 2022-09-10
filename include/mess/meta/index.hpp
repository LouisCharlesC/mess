// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <cstdint>
#include <tuple>

namespace mess
{
namespace details
{
template <typename tag> struct index_finder_t
{
    constexpr index_finder_t(std::size_t index) : index(index){};
    std::size_t index;
};

template <typename flat_graph, std::size_t... indexes> consteval auto make_index_finder(std::index_sequence<indexes...>)
{
    return std::tuple<index_finder_t<typename std::tuple_element_t<indexes, flat_graph>::tag>...>(indexes...);
}

template <typename flat_graph>
constexpr index_finder_t<flat_graph> index_finder =
    make_index_finder<flat_graph>(std::index_sequence_for<flat_graph>());
} // namespace details

template <typename tag, typename flat_graph>
constexpr std::size_t index = std::get<tag, details::index_finder<flat_graph>>().index;
} // namespace mess
