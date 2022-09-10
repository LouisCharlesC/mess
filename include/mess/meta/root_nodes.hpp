/**
 * @file root_nodes.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-16
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
constexpr bool is_root = std::tuple_element_t<index, flat_graph>::unordered_predecessors::size == 0;

template <typename flat_graph, std::size_t... indexes_v>
using root_nodes = concatenate<std::conditional_t<is_root<flat_graph, indexes_v>, indexes<indexes_v>, indexes<>>...>;
} // namespace mess
