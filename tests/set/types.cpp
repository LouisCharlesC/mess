// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <set/types.hpp>

static_assert(types::is_unique<int, int, float>, "");
static_assert(types::is_unique<int, float, int>, "");
static_assert(types::is_unique<int, float, int, double>, "");
static_assert(!types::is_unique<int, float>, "");
static_assert(!types::is_unique<int, int, float, int>, "");
static_assert(!types::is_unique<int, float, int, int>, "");

static_assert(types::all_unique<>, "");
static_assert(types::all_unique<int, float, double>, "");
static_assert(!types::all_unique<int, int, float, int>, "");
static_assert(!types::all_unique<float, int, int>, "");
static_assert(!types::all_unique<float, double, int, int, double>, "");

static_assert(types::all_unique<std::tuple<>>, "");
static_assert(types::all_unique<std::tuple<int, float, double>>, "");
static_assert(!types::all_unique<std::tuple<int, int, float, int>>, "");
static_assert(!types::all_unique<std::tuple<float, int, int>>, "");
static_assert(!types::all_unique<std::tuple<float, double, int, int, double>>, "");

static_assert(types::details::is_tuple_like<std::tuple<>>, "");
static_assert(types::details::is_tuple_like<std::tuple<double>>, "");
static_assert(!types::details::is_tuple_like<double>, "");

static_assert(types::Set<std::tuple<>>, "");
static_assert(types::Set<std::tuple<int, float, double>>, "");
static_assert(!types::Set<std::tuple<int, int, float, int>>, "");
static_assert(!types::Set<std::tuple<float, int, int>>, "");
static_assert(!types::Set<std::tuple<float, double, int, int, short>>, "");
static_assert(!types::Set<float>, "");
