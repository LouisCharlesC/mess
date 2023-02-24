// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <dag/leafs.hpp>

#include <array>
#include <type_traits>
#include <vector>

namespace
{
struct One;
struct Two;
struct Three;
} // namespace

using graph = dag::graph<dag::node<One(Three, Two)>, dag::node<Two()>, dag::node<Three()>>;

static_assert(std::is_same_v<dag::leafs<graph>, set::types<One>>, "");

template <std::array<std::size_t, 3> indexes, typename Is = std::make_index_sequence<indexes.size()>> struct for_each;
template <std::array<std::size_t, 3> indexes, std::size_t... Is> struct for_each<indexes, std::index_sequence<Is...>>
{
    template <typename F> static constexpr int apply(F &&f)
    {

        (f<Is>(indexes[Is]), ...);

        return 0;
    }
};

static std::array<std::size_t, 3> out;

auto f = []<std::size_t i>(std::size_t v) { out[i] = v; };

constexpr std::array<std::size_t, 3> in = {3, 2, 1};

// constexpr int grr = for_each<in>(f);
int grrr = for_each<in>::apply(f);

for_each<in>::apply([]<std::size_t index>() { execute_if_ready<> });
