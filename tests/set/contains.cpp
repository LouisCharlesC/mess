// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <set/contains.hpp>
#include <set/types.hpp>

static_assert(set::contains<int, set::types<int, float>>, "");
static_assert(set::contains<set::types<int, float>, float, int>, "");
static_assert(!set::contains<set::types<float>, int>, "");
static_assert(!set::contains<set::types<int>, set::types<float>>, "");
