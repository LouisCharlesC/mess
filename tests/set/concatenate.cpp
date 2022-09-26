// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <set/concatenate.hpp>
#include <set/types.hpp>

static_assert(std::is_same_v<set::concatenate<set::indexes<3>, set::indexes<1>>, set::indexes<3, 1>>, "");
static_assert(std::is_same_v<set::concatenate<set::indexes<1, 2, 3>, set::indexes<>>, set::indexes<1, 2, 3>>, "");
