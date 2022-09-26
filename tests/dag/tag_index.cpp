// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <dag/tag_index.hpp>

struct One;
struct Two;
struct Three;
struct Four;
struct Five;

using caret_graph = dag::graph<dag::node<One>, dag::node<Two, One>, dag::node<Three, One>>;

static_assert(dag::index_of<caret_graph, One> == 0, "");
static_assert(dag::index_of<caret_graph, Two> == 1, "");
static_assert(dag::index_of<caret_graph, Three> == 2, "");
