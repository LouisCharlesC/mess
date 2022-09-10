// Copyright(c) 2022 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

namespace mess
{
template <typename... tags_t> struct tags
{
    static constexpr std::size_t size = sizeof...(tags_t);

    template <typename... other_tags_t> constexpr tags<tags_t..., other_tags_t...> operator+(tags<other_tags_t...>)
    {
        return {};
    }
};

template <std::size_t... indexes_v> struct indexes
{
    static constexpr std::size_t size = sizeof...(indexes_v);

    template <std::size_t... other_indexes_v>
    constexpr indexes<indexes_v..., other_indexes_v...> operator+(indexes<other_indexes_v...>)
    {
        return {};
    }
};

template <typename... tags_t> struct arg_predecessors : tags<tags_t...>
{
};
template <typename... tags_t> struct other_predecessors : tags<tags_t...>
{
};
template <typename... tags_t> struct successors : tags<tags_t...>
{
};

template <typename... tags_t> constexpr bool is_arg_predecessors_type(arg_predecessors<tags_t...>)
{
    return true;
}
template <typename... tags_t> constexpr bool is_other_predecessors_type(other_predecessors<tags_t...>)
{
    return true;
}
template <typename... tags_t> constexpr bool is_successors_type(successors<tags_t...>)
{
    return true;
}
} // namespace mess