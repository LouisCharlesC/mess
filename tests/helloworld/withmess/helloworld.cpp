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

// static const char* kHelloWorld = "Hello, world!\n";

// using PrintFnPtr = std::basic_ostream<char, std::char_traits<char>>&(*)(std::basic_ostream<char, std::char_traits<char>>&, const char*);
// static PrintFnPtr print = std::operator<< <std::char_traits<char> >;

// struct PrintHelloWorld:
// 	mess::IsTheResultOfCalling<&print>,
// 	mess::WithArguments<
// 		mess::IsPointedToBy<&std::cout>,
// 		mess::IsPointedToBy<&kHelloWorld>>
// {};

int main()
{
	// mess::pull<PrintHelloWorld>();
}