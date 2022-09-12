// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <doctest/doctest.h>

#include <mess/meta/contains.hpp>

#include <type_traits>

TEST_CASE("Empty contained")
{
    static_assert(mess::contains<mess::indexes<>, mess::indexes<0, 1, 2>>, "");
}