// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <set/contains.hpp>
#include <set/indexes.hpp>
#include <set/types.hpp>

struct One;
struct Two;
struct Three;

static_assert(indexes::contains(indexes::set<2ul, 1ul>, indexes::set<1ul>), "");
static_assert(indexes::contains(indexes::set<1ul, 2ul>, indexes::set<2ul>), "");
static_assert(indexes::contains(indexes::set<1ul, 2ul, 3ul>, indexes::set<>), "");
static_assert(indexes::contains(indexes::set<>, indexes::set<>), "");
static_assert(!indexes::contains(indexes::set<1ul, 2ul>, indexes::set<3ul, 1ul>), "");
static_assert(!indexes::contains(indexes::set<>, indexes::set<3ul>), "");

static_assert(types::contains<types::set<Two, One>, types::set<One>>, "");
static_assert(types::contains<types::set<One, Two>, types::set<Two>>, "");
static_assert(types::contains<types::set<One, Two, Three>, types::set<>>, "");
static_assert(types::contains<types::set<>, types::set<>>, "");
static_assert(!types::contains<types::set<One, Two>, types::set<Three, One>>, "");
static_assert(!types::contains<types::set<>, types::set<Three>>, "");
