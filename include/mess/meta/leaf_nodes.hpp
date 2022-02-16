/**
 * @file leaf_nodes.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <mess/meta/concatenate.hpp>

#include <cstdint>
#include <tuple>
#include <type_traits>

namespace mess
{
    template <typename flat_graph, std::size_t index>
    constexpr bool is_leaf()
    {
        return std::tuple_element_t<index, flat_graph>::successors::size() == 0;
    }

    template <typename flat_graph, std::size_t... indexes>
    using leaf_nodes = concatenate<std::conditional_t<is_leaf<flat_graph, indexes>(), std::index_sequence<indexes>, std::index_sequence<>>...>;
} // namespace mess