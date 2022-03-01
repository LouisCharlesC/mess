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

#include <mess/schedulers/inline.hpp>
#include <mess/latches/atomic_countdown.hpp>
#include <mess/latches/last_predecessor.hpp>

#include <optional>

namespace mess
{
    template <typename scheduler_type>
    struct kit_customizer
    {
        template <std::size_t... predecessors>
        using latch_type = atomic_countdown_latch<predecessors...>;
        template <typename value_type>
        using result_type = std::optional<value_type>;
    };

    template <>
    struct kit_customizer<inline_scheduler>
    {
        template <std::size_t... predecessors>
        using latch_type = last_predecessor_latch<predecessors...>;
        template <typename value_type>
        using result_type = std::optional<value_type>;
    };
} // namespace mess