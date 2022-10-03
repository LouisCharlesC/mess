// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <dag/tags.hpp>

struct One;
struct Two;
struct Three;

using caret_graph = dag::graph<dag::node<One()>, dag::node<Two(One)>, dag::node<Three(One)>>;

static_assert(std::is_same_v<dag::tags<caret_graph>, set::types<One, Two, Three>>, "");
// static_assert(std::is_same_v<dag::node<One, Two, Three>::predecessors, type_set::type_set<Two, Three>>, "");
