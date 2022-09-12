// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <mess/kit.hpp>

#include <utility>

namespace mess
{
template <typename scheduler_type, typename flat_graph> class frame_type
{
  public:
    frame_type(scheduler_type &scheduler, flat_graph graph) : _scheduler(scheduler), _graph(std::move(graph))
    {
    }

    // private:
    [[no_unique_address]] flat_graph _graph;
    [[no_unique_address]] runtime_state<scheduler_type, flat_graph> _runtime;
    [[no_unique_address]] leaf_nodes_latch<scheduler_type, flat_graph> _leafs_latch;
    scheduler_type &_scheduler; // TODO copy the scheduler
};
} // namespace mess