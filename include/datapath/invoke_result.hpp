/**
 * @file invoke_result.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "find.hpp"
#include "sequences.hpp"

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace mess
{
namespace details
{
// We need a template struct specialization to get a parameter pack, so first declare the template.
template <typename flat_graph, std::size_t index, typename> struct invoke_result;
// And forward declare the specialization we need.
template <typename flat_graph, std::size_t index, typename... arg_predecessor_tags>
struct invoke_result<flat_graph, index, mess::arg_predecessors<arg_predecessor_tags...>>;

// Convenience alias to the type defined in the specialization.
template <typename flat_graph, std::size_t index>
using invoke_result_t = typename invoke_result<
    flat_graph, index,
    mess::to_indexes<flat_graph, typename std::tuple_element_t<index, flat_graph>::arg_predecessor_tags>>::type;

// The actual definition of the specialization.
template <typename flat_graph, std::size_t index, std::size_t... arg_predecessor>
struct invoke_result<flat_graph, index, indexes<arg_predecessor...>>
{
    // Recursevely get the invoke result of the predecessors.
    using type = std::invoke_result_t<typename std::tuple_element_t<index, flat_graph>::invocable_type,
                                      invoke_result_t<flat_graph, arg_predecessor>...>;
};
} // namespace details
} // namespace mess