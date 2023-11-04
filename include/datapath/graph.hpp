// Copyright(c) 2023 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

// #include "dag/graph.hpp"

#include <utility>

namespace dpath
{
// Node of a directed acyclic datapath
template <typename signature, typename invocable_t> struct node;
template <typename tag, typename... predecessors, typename invocable_t> class node<tag(predecessors...), invocable_t>
{
    // dag::node will check everything we can.
    // using dag_node = dag::node<tag(predecessors...)>;

  public:
    template <typename invocable_fwd>
    node(invocable_fwd &&invocable) : _invocable(std::forward<invocable_fwd>(invocable))
    {
    }

  private:
    invocable_t _invocable;
};

struct promise
{
    void detach()
    {
    }
};

template <typename... nodes> struct graph
{
    promise run()
    {
        return promise();
    }
}; // = types::set<nodes...>;

namespace builders
{
template <typename signature, typename invocable_t> auto node(invocable_t &&invocable)
{
    return dpath::node<signature, invocable_t>(std::forward<invocable_t>(invocable));
}
template <typename... nodes> auto graph(nodes &&...)
{
    return dpath::graph<nodes...>();
}
} // namespace builders

} // namespace dpath
