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

struct Logger:
    mess::Call<coutLogger>, // replace with: "mess::Call<cerrLogger>," to change the logger!
    mess::WithNoArgumentss
{};

int main(int argc, char **argv)
{
     mess::Frame::pull<Logger>() << "Hello, world!\n";
}