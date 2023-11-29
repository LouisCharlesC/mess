// Copyright(c) 2023 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <datapath/graph.hpp>

struct Top;
struct Left;
struct Right;
struct Bottom;

using namespace dpath;

struct One
{
};
struct Two
{
};
struct Three
{
};
struct Four
{
};

One top()
{
    return {};
}
Two left(One)
{
    return {};
}
Three right(One)
{
    return {};
}
Four bottom(Two, Three)
{
    return {};
}

//     top
//   /      \
// left    right
//   \      /
//    bottom

// auto allo = node<Top()>(top);
auto upper_left_diamond = make::graph(make::node<Top()>(top), make::node<Left(Top)>(left));
auto lower_right_diamond = make::graph(make::node<Right(Top)>(right), make::node<Bottom(Left, Right)>(bottom));
auto diamond = make::graph(upper_left_diamond, lower_right_diamond);

// it does not return anything. If you want to cancel from outside: use the executor. If you want to customize what
// kind of execution fancy thing you wanna do, that's also on the executor.
// Can the executor also keep everything alive ? Like it's a task they execute... no unique pointer shenanigans. No,
// that would require waiting

// mess::async(graph, executor);
// auto frame = mess::run(graph, executor);