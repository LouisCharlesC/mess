// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "types.hpp"

namespace set
{
namespace details
{
// The class template can perform concatenation by addition.
template <typename... types> struct concatenator
{
    using concatenate = set::types<types...>;

    template <typename... rhs_types>
    constexpr concatenator<types..., rhs_types...> operator+(concatenator<rhs_types...>)
    {
        return {};
    }
};
// This specialization exists just to access the types parameter pack from the set::types.
template <typename... types> struct concatenator<set::types<types...>>
{
    using type = details::concatenator<types...>;
};
} // namespace details

template <TypeSet... type_sets>
using concatenate =
    typename decltype((std::declval<typename details::concatenator<type_sets>::type>() + ...))::concatenate;

} // namespace set