// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <tuple_ops/concatenate.hpp>

#include <tuple>
#include <type_traits>

static_assert(std::is_same_v<tuple_ops::concatenate<std::tuple<int>, std::tuple<float>>, std::tuple<int, float>>, "");
static_assert(
    std::is_same_v<tuple_ops::concatenate<std::tuple<int>, std::tuple<float, double>>, std::tuple<int, float, double>>,
    "");
static_assert(std::is_same_v<tuple_ops::concatenate<std::tuple<int>, std::tuple<float>, std::tuple<double>>,
                             std::tuple<int, float, double>>,
              "");
