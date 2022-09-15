/**
 * @file kit_customizer.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <mess/latches/atomic_countdown.hpp>
#include <mess/latches/last_predecessor.hpp>
#include <mess/schedulers/inline.hpp>

#include <functional>
#include <optional>
#include <type_traits>

namespace mess
{
template <typename scheduler_type> struct kit_customizer
{
    template <std::size_t... predecessors> using latch_type = atomic_countdown_latch<predecessors...>;

    using cancel_flag_type = std::atomic<bool>;

    template <typename value_type>
    using result_type = std::conditional_t<std::is_lvalue_reference_v<value_type>,
                                           std::optional<std::reference_wrapper<std::remove_reference_t<value_type>>>,
                                           std::optional<value_type>>;
};

template <> struct kit_customizer<inline_scheduler_t>
{
    template <std::size_t... predecessors> using latch_type = last_predecessor_latch<predecessors...>;

    using cancel_flag_type = bool;

    template <typename value_type>
    using result_type = std::conditional_t<std::is_lvalue_reference_v<value_type>,
                                           std::optional<std::reference_wrapper<std::remove_reference_t<value_type>>>,
                                           std::optional<value_type>>;
};
} // namespace mess