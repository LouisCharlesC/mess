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

namespace { // avoids including this function in the binary!
    std::ostream& getLogger()
    {
        return std::cout;
    }
}

struct Logger:
    mess::Call<getLogger>,
    mess::WithNoArgument
{};

int main(int argc, char **argv)
{
     mess::pull<Logger>() << "Hello, world!\n";
}