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
    std::ostream& getCout()
    {
        return std::cout;
    }
    const char* getHelloWorld()
    {
        return "Hello, world!\n";
    }
}

struct StdCout:
    mess::IsTheResultOfCalling<getCout>,
    mess::WithNoArgument
{};
struct HelloWorld:
    mess::IsTheResultOfCalling<getHelloWorld>,
    mess::WithNoArgument
{};
struct PrintHelloWorld:
    // Sorry for the line below, but using an operator from the std namespace proves the non-intrusiveness of mess!
    // And demonstrates a current limitation of mess: you must manually resolve overloads and provide template arguments.
    // So, here's a static cast to a function pointer to the overload-resolved, template-provided std::operator<<().
    mess::IsTheResultOfCalling<static_cast<std::basic_ostream<char, std::char_traits<char>>&(*)(std::basic_ostream<char, std::char_traits<char>>&, const char*)>(std::operator<<<std::char_traits<char>>)>, 
    mess::WithArguments<StdCout, HelloWorld>
{};

int main(int argc, char **argv)
{
     mess::pull<PrintHelloWorld>();
}