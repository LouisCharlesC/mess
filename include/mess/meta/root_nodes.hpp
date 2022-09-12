/**
 * @file root_tags.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "concatenate.hpp"
#include "sequences.hpp"

#include <cstdint>
#include <tuple>
#include <type_traits>
#include <utility>

namespace mess
{
namespace details
{
template <typename flat_graph, std::size_t index>
constexpr bool is_root = std::tuple_element_t<index, flat_graph>::arg_predecessor_tags::size +
                             std::tuple_element_t<index, flat_graph>::other_predecessor_tags::size ==
                         0;

template <typename flat_graph, std::size_t... all_indexes>
consteval auto root_indexes(std::index_sequence<all_indexes...>)
{
    return concatenate<std::conditional_t<is_root<flat_graph, all_indexes>, indexes<all_indexes>, indexes<>>...>();
}
} // namespace details

template <typename flat_graph>
using root_indexes =
    decltype(details::root_indexes<flat_graph>(std::make_index_sequence<std::tuple_size_v<flat_graph>>()));
} // namespace mess
