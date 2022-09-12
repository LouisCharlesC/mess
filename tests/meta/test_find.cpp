// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <doctest/doctest.h>

#include <mess/mess.hpp>
#include <mess/meta/find.hpp>

#include <cassert>
#include <type_traits>

namespace
{
mess::inline_scheduler_t scheduler = mess::inline_scheduler;

struct First;
struct Second;
struct Last;

template <typename Return, typename... Args> Return func(Args...)
{
    return {};
}
} // namespace

TEST_CASE("Three nodes")
{
    constexpr auto three_nodes = mess::make_graph(mess::make_node<First, mess::arg_predecessors<>>(func<uint>),
                                                  mess::make_node<Second, mess::arg_predecessors<>>(func<uint>),
                                                  mess::make_node<Last, mess::arg_predecessors<>>(func<uint>));
    using three_nodes_t = decltype(three_nodes);

    SUBCASE("Find first")
    {
        CHECK_EQ(mess::find<three_nodes_t, First>, 0);
    }
    SUBCASE("Find second")
    {
        CHECK_EQ(mess::find<three_nodes_t, Second>, 1);
    }
    SUBCASE("Find last")
    {
        CHECK_EQ(mess::find<three_nodes_t, Last>, 2);
    }
}
