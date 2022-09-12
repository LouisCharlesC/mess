// Copyright(c) 2021-2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <mess/frame.hpp>
#include <mess/graph.hpp>

#include <mess/details/helpers.hpp>

#include <memory>
#include <utility>

namespace mess
{
template <typename frame_type> void run(frame_type &&frame)
{
    details::invoker<false>::schedule_root_nodes(frame);
}

template <typename scheduler_type, typename... nodes_type>
void run(std::unique_ptr<frame_type<scheduler_type, flat_graph<nodes_type...>>> ptr)
{
    details::invoker<true>::schedule_root_nodes(*ptr.release());
}
} // namespace mess
