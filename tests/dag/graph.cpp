// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <dag/graph.hpp>

struct One;
struct Two;
struct Three;
struct Four;
struct Five;

static_assert(std::is_same_v<dag::node<One, Two, Three>::tag, One>, "");
static_assert(std::is_same_v<dag::node<One, Two, Three>::predecessors, set::types<Two, Three>>, "");
