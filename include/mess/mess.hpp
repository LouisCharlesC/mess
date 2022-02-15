/**
 * @file mess.h
 * @author L.-C. C. (lccaronlc@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-12-02
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <mess/details/helpers.hpp>
#include <mess/frame.hpp>

#include <memory>
#include <utility>

namespace mess
{
    template <typename arg_predecessors_type, typename await_predecessors_type, typename successors_type, typename invocable_type>
    auto make_node(invocable_type invocable)
    {
        return node_type<invocable_type, arg_predecessors_type, await_predecessors_type, successors_type>{std::forward<invocable_type>(invocable)};
    }

    template <typename... nodes_type>
    auto make_graph(nodes_type &&...nodes)
    {
        return std::make_tuple(std::forward<nodes_type>(nodes)...);
    }

    template <typename executor_type, typename... nodes_type>
    void run(frame_type<executor_type, flat_graph<nodes_type...>> &frame)
    {
        details::self_delete<false>::execute_root_nodes(frame);
    }

    template <typename executor_type, typename... nodes_type>
    void run(std::unique_ptr<frame_type<executor_type, flat_graph<nodes_type...>>> ptr)
    {
        details::self_delete<true>::execute_root_nodes(*ptr.release());
    }
} // namespace mess
