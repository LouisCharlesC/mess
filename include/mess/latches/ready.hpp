/**
 * @file ready.hpp
 * @author L.-C. C. (me@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <cstddef>

namespace mess
{
template <std::size_t predecessor> class ready_latch
{
  public:
    template <std::size_t> bool notify_and_check_if_ready()
    {
        return true;
    }
};
} // namespace mess
