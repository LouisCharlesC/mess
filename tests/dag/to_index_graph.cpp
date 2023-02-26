// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <dag/to_index_graph.hpp>

struct One;
struct Two;
struct Three;

using one = dag::node<One()>;
using two = dag::node<Two(One)>;
using three = dag::node<Three(Two, One)>;
using one_two_three = dag::graph<one, two, three>;

static_assert(dag::to_index_node<types::set<One, Two, Three>, one> == dag::indexes::node<0>);
static_assert(dag::to_index_node<types::set<One, Two, Three>, two> == dag::indexes::node<1, 0>);

constexpr auto allo = dag::to_index_graph<one_two_three>;

static_assert(dag::to_index_graph<one_two_three> ==
              dag::indexes::graph<dag::indexes::node<0>, dag::indexes::node<1, 0>, dag::indexes::node<2, 1, 0>>);
