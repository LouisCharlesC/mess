// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <set/indexes.hpp>

static_assert(indexes::details::all_unique(indexes::array<0>()), "");
static_assert(indexes::details::all_unique(indexes::set<>), "");
static_assert(indexes::details::all_unique(indexes::set<1ul>), "");
static_assert(indexes::details::all_unique(indexes::set<1ul, 3ul>), "");
static_assert(indexes::details::all_unique(indexes::set<2ul, 1ul, 3ul>), "");
