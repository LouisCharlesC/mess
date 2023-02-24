// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <dag/tags.hpp>

struct One;
struct Two;
struct Three;

static_assert(std::is_same_v<dag::tags<dag::graph<dag::node<One()>, dag::node<Two(One)>, dag::node<Three(One)>>>,
                             types::set<One, Two, Three>>,
              "");
static_assert(std::is_same_v<dag::tags<dag::graph<dag::node<Two(Three)>, dag::node<One()>, dag::node<Three(One)>>>,
                             types::set<Two, One, Three>>,
              "");
