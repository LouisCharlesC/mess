// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "indexes.hpp"

namespace set
{
template <auto indexes, typename Is = std::make_index_sequence<indexes.size()>> struct for_each;
template <auto indexes, std::size_t... Is>
requires is_index_set<indexes>
struct for_each<indexes, std::index_sequence<Is...>>
{
    static constexpr auto apply(auto &&collection, auto &&map, auto &&reduce)
    {
        return;
    }
};
} // namespace set
