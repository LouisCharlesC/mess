// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <dag/to_indexes.hpp>

struct One;
struct Two;
struct Three;

static_assert(std::is_same_v<dag::node<One(Two, Three)>::tag, One>, "");
static_assert(std::is_same_v<dag::node<One(Two, Three)>::predecessors, types::set<Two, Three>>, "");

static_assert(dag::Graph<types::set<dag::node<One(Two, Three)>, dag::node<Two(One, Three)>>>,
              ""); // this should not be anymore once I can check local loops
// static_assert(!dag::Graph<types::set<dag::node<One(Two, Three)>, dag::node<One(Two, Three)>>>, ""); // sadly this
// does not compile
static_assert(!dag::Graph<types::set<dag::node<One(Two, Three)>, dag::node<One(Three, Two)>>>, "");
