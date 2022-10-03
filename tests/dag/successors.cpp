// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <dag/successors.hpp>
#include <dag/tag_index.hpp>

#include <type_traits>

struct One;
struct Two;
struct Three;

using graph = dag::graph<dag::node<One()>, dag::node<Two(One)>, dag::node<Three(One)>>;
using v_graph = dag::graph<dag::node<One()>, dag::node<Two()>, dag::node<Three(One, Two)>>;

static_assert(std::is_same_v<dag::successors<graph, One>, set::types<Two, Three>>, "");
static_assert(std::is_same_v<dag::successors<graph, Two>, set::types<>>, "");
static_assert(std::is_same_v<dag::successors<graph, Three>, set::types<>>, "");

static_assert(std::is_same_v<dag::successors<v_graph, One>, set::types<Three>>, "");
static_assert(std::is_same_v<dag::successors<v_graph, Two>, set::types<Three>>, "");
static_assert(std::is_same_v<dag::successors<v_graph, Three>, set::types<>>, "");
