// Copyright(c) 2021-2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <mess/details/helpers.hpp>
#include <mess/frame.hpp>

#include <memory>
#include <utility>

namespace mess
{
template <typename tag, typename arg_predecessors_tags, typename successors = mess::successors<>,
          typename other_predecessors_tags = mess::other_predecessors<>, typename invocable_type = void>
auto make_node(invocable_type &&invocable)
{
    return node_type<tag, invocable_type, arg_predecessors_tags, successors, other_predecessors_tags>{
        std::forward<invocable_type>(invocable)};
}

template <typename... nodes_type> auto make_graph(nodes_type &&...nodes)
{
    return std::make_tuple(std::forward<nodes_type>(nodes)...);
}

template <typename frame_type> void run(frame_type &&frame)
{
    details::invoker<false>::scheduler_root_nodes(frame);
}

template <typename scheduler_type, typename... nodes_type>
void run(std::unique_ptr<frame_type<scheduler_type, flat_graph<nodes_type...>>> ptr)
{
    details::invoker<true>::scheduler_root_nodes(*ptr.release());
}
} // namespace mess
