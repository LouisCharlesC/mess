// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <set/index_of.hpp>
#include <set/types.hpp>

struct One;
struct Two;
struct Three;

using one_two_three = types::set<One, Two, Three>;
using one_three = types::set<One, Three>;

static_assert(types::index_of<one_two_three, One> == 0, "");
// static_assert(types::index_of<one_two_three, Two> == 1, "");
// static_assert(types::index_of<one_two_three, Three> == 2, "");

// static_assert(types::to_indexes<one_two_three, one_three> == indexes::set<0, 2>, "");
