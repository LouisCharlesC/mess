// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "is_tuple.hpp"

namespace tuple_ops
{
namespace details
{
template <typename... types> struct concatenate_by_adding
{
    using tuple = std::tuple<types...>;

    constexpr concatenate_by_adding(std::tuple<types...>)
    {
    }

    template <typename... rhs_types>
    constexpr concatenate_by_adding<types..., rhs_types...> operator+(concatenate_by_adding<rhs_types...>)
    {
        return {};
    }
};
} // namespace details

template <typename... tuples>
requires(is_tuple<tuples> &&...) using concatenate = typename decltype((details::concatenate_by_adding(tuples()) +
                                                                        ...))::tuple;
} // namespace tuple_ops