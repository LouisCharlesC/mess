// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <set/types.hpp>

struct One;

static_assert(set::is_unique<int, int, float>, "");
static_assert(set::is_unique<int, float, int>, "");
static_assert(set::is_unique<int, float, int, double>, "");
static_assert(!set::is_unique<int, float>, "");
static_assert(!set::is_unique<int, int, float, int>, "");
static_assert(!set::is_unique<int, float, int, int>, "");

static_assert(set::all_unique<int, float, double>, "");
static_assert(!set::all_unique<int, int, float, int>, "");
static_assert(!set::all_unique<float, int, int>, "");
static_assert(!set::all_unique<float, double, int, int, double>, "");

static_assert(set::is_type_set<std::tuple<One>>, "");
static_assert(set::is_type_set<std::tuple<int, float, double>>, "");
static_assert(!set::is_type_set<std::tuple<int, int, float, int>>, "");
static_assert(!set::is_type_set<std::tuple<float, int, int>>, "");
static_assert(!set::is_type_set<std::tuple<float, double, int, int, double>>, "");
static_assert(!set::is_type_set<float>, "");
