// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <doctest/doctest.h>

#include <sequence/sequence.hpp>

#include <cassert>
#include <type_traits>

template <typename... types> using type_set = sequence::type_set<types...>;
template <std::size_t index> using index_constant = sequence::index_constant<index>;
template <std::size_t... indexes> using index_set = sequence::index_set<indexes...>;

TEST_CASE("type_set size")
{
    static_assert(type_set<int>::size == 1, "");
    static_assert(type_set<int, int>::size == 2, "");
}

TEST_CASE("type_set concat")
{
    static_assert(std::is_same_v<type_set<int>::concatenate<type_set<float>>, type_set<int, float>>, "");
    static_assert(
        std::is_same_v<type_set<int, float, double>::concatenate<type_set<char>>, type_set<int, float, double, char>>,
        "");
}

TEST_CASE("type_set contains")
{
    static_assert(type_set<int, float>::contains<type_set<int>>, "");
    static_assert(type_set<int, float>::contains<type_set<float, int>>, "");
    static_assert(!type_set<int>::contains<type_set<float>>, "");
}

TEST_CASE("index_set size")
{
    static_assert(index_set<1>::size == 1, "");
    static_assert(index_set<3, 1>::size == 2, "");
}

TEST_CASE("type_set concat")
{
    static_assert(std::is_same_v<index_set<3>::concatenate<index_set<1>>, index_set<3, 1>>, "");
    static_assert(std::is_same_v<index_set<1, 2, 3>::concatenate<index_set<3>>, index_set<1, 2, 3, 3>>, "");
}

TEST_CASE("type_set contains")
{
    static_assert(index_set<1, 3>::contains<index_set<1>>, "");
    static_assert(index_set<3, 1>::contains<index_set<1, 3>>, "");
    static_assert(!index_set<1>::contains<index_set<3>>, "");
}

TEST_CASE("mixed concat")
{
    static_assert(std::is_same_v<type_set<int>::concatenate<index_set<1>>, type_set<int, index_constant<1>>>, "");
    static_assert(
        std::is_same_v<type_set<index_constant<1>, index_constant<2>>::concatenate<index_set<3>>, index_set<1, 2, 3>>,
        "");
}

TEST_CASE("mixed contains")
{
    static_assert(index_set<1, 3>::contains<index_set<1>>, "");
    static_assert(index_set<3, 1>::contains<index_set<1, 3>>, "");
    static_assert(!index_set<1>::contains<index_set<3>>, "");
}