// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <set/disjunction.hpp>
#include <set/types.hpp>

// set::disjunction<set::type_set<int>, set::type_set<float>>::a;

static_assert(std::is_same_v<set::disjunction<>, set::type_set<>>, "");
static_assert(std::is_same_v<set::disjunction<set::type_set<>>, set::type_set<>>, "");
static_assert(std::is_same_v<set::disjunction<set::type_set<>, set::type_set<>>, set::type_set<>>, "");
static_assert(std::is_same_v<set::disjunction<set::type_set<int>, set::type_set<>>, set::type_set<int>>, "");
static_assert(std::is_same_v<set::disjunction<set::type_set<>, set::type_set<int>>, set::type_set<int>>, "");
// static_assert(std::is_same_v<set::disjunction<set::type_set<int>, set::type_set<float>>, set::type_set<int, float>>,
//   "");
// static_assert(std::is_same_v<set::disjunction<set::indexes<3>>, set::indexes<3>>, "");
// static_assert(std::is_same_v<set::disjunction<set::indexes<3>, set::indexes<>>, set::indexes<3>>, "");
// static_assert(std::is_same_v<set::disjunction<set::indexes<>, set::indexes<1>>, set::indexes<1>>, "");
// static_assert(std::is_same_v<set::disjunction<set::indexes<1, 2, 3>, set::indexes<>>, set::indexes<1, 2, 3>>, "");
// static_assert(std::is_same_v<set::disjunction<set::indexes<1, 2, 3>, set::indexes<1, 2>>, set::indexes<1, 2, 3>>,
// "");
