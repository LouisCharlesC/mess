// Copyright(c) 2023 Louis-Charles Caron

// This file is part of the mess library (https://github.com/LouisCharlesC/mess).

// Use of this source code is governed by an MIT-style license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <datapath/graph.hpp>

struct Top;
struct Left;
struct Right;
struct Bottom;

using namespace dpath::builders;

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
auto diamond =
    graph(node<Top()>(top), node<Left(Top)>(left), node<Right(Top)>(right), node<Bottom(Left, Right)>(bottom));

auto promise = diamond.run(executor);

promise.detach();