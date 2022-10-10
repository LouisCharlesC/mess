// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <utility>

namespace mess
{
template <typename... sequences_t> using disjunction = decltype((std::declval<sequences_t>() + ...));
} // namespace mess