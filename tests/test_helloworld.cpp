// Copyright(c) 2019-2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <doctest/doctest.h>

#include <algorithm>
#include <fstream>
#include <vector>

TEST_CASE("Helloworld compiles to same binary with or without mess")
{
    std::filebuf fb;
    fb.open("helloworld", std::ios::in | std::ios::binary);
    if (!fb.is_open()) // maybe this is Windows...
    {
        fb.open("Release\\helloworld.exe", std::ios::in | std::ios::binary);
    }
    REQUIRE(fb.is_open());
    const auto sizeWithMess = fb.in_avail();
    REQUIRE(sizeWithMess >= 0);
    std::vector<char> withMess(static_cast<std::vector<char>::size_type>(sizeWithMess));
    fb.sgetn(withMess.data(), sizeWithMess);
    fb.close();
    fb.open("helloworldwomess", std::ios::in | std::ios::binary);
    if (!fb.is_open()) // maybe this is Windows...
    {
        fb.open("Release\\helloworldwomess.exe", std::ios::in | std::ios::binary);
    }
    REQUIRE(fb.is_open());
    const auto sizeWithoutMess = fb.in_avail();
    REQUIRE(sizeWithoutMess >= 0);
    std::vector<char> withoutMess(static_cast<std::vector<char>::size_type>(sizeWithoutMess));
    fb.sgetn(withoutMess.data(), sizeWithoutMess);

    REQUIRE_EQ(sizeWithMess, sizeWithoutMess);
    CHECK(std::equal(withMess.cbegin(), withMess.cend(), withoutMess.cbegin()));
}
