/**
 * @file last_predecessor.h
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-05
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <mess/meta/list.hpp>

#include <cstdint>

namespace mess
{
    namespace details
    {
        template <std::size_t v>
        struct get
        {
            static constexpr std::size_t value = v;
        };
        template <std::size_t index, std::size_t... indexes>
        static constexpr bool is_last(list<indexes...>)
        {
            // the <()> syntax uses the comma operator of std::size_t to only use the last item, effectively resulting in get<last_index>
            return index == get<(indexes, ...)>::value;
        }
        template <std::size_t>
        static constexpr bool is_last(list<>)
        {
            return false;
        }
    } // namespace details

    template <std::size_t... predecessors>
    struct last_predecessor_latch
    {
        template <std::size_t notifying>
        constexpr bool notify_and_check_if_ready() const noexcept
        {
            // FIXME: that ain't gonna work, I need to sort the indexes in order of execution (depth first walk), then check the last.
            return is_last<notifying>(details::list<predecessors...>());
        }
    };
} // namespace mess