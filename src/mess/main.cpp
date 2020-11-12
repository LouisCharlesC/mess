/**
 * @file main.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-10-27
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <cassert>
#include <iostream>

void call(int) {}

void func(int other_funcs)
{
	call(other_funcs);
	// outlive!
}

int main()
{
	return EXIT_SUCCESS;
}