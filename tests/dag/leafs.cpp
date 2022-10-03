// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <dag/leafs.hpp>

#include <type_traits>

namespace
{
struct One;
struct Two;
struct Three;
} // namespace

using graph = dag::graph<dag::node<One(Three, Two)>, dag::node<Two()>, dag::node<Three()>>;

static_assert(std::is_same_v<dag::leafs<graph>, set::types<One>>, "");