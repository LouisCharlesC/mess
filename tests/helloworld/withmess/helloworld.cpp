/**
 * @file helloworld.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-04-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <mess/mess.h>

#include <iostream>

namespace {
std::ostream& coutLogger()
{
    return std::cout;
}
std::ostream& cerrLogger()
{
    return std::cerr;
}
}

struct Logger;

namespace mess
{
    template<> struct
    ToProduce<Logger>:
        Call<coutLogger>, // replace with: "Call<cerrLogger>," to change the logger!
        WithNoArgs
    {};
}

int main(int argc, char **argv)
{
     mess::Frame<>::pull<Logger>() << "Hello, world!\n";
}