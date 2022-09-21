// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <tuple>

namespace tuple_ops
{
template <typename type> constexpr bool is_tuple = false;
template <typename... types> constexpr bool is_tuple<std::tuple<types...>> = true;
} // namespace tuple_ops