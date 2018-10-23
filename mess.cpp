//============================================================================
// Name        : mess.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "broker.h"

#include <iostream>
#include <vector>


//void allo( int i) {std::cout << i << std::endl;}

int main() {
//	void(*ptr)(int)  = &allo;
//
//	std::vector<void(*)()> vec;
//	vec.push_back(reinterpret_cast<void(*)()>(ptr));
//
//	(*reinterpret_cast<void(*)(int)>(vec[0]))(5);

	mess::Broker<std::string> broker;
	broker.subscribe<float>("allo", [](const float& msg){std::cout << msg << std::endl;});
	broker.subscribe<float>("allo", [](float&& msg){std::cout << (msg+=2.f) << std::endl;});
	broker.subscribe<float>("allo", [](float&& msg){std::cout << (msg+=2.f) << std::endl;});
	broker.subscribe<float>("allo", [](const float& msg){std::cout << msg << std::endl;});

	float msg = 2;
	broker.publish<float>("allo", std::move(msg));
	std::cout << msg << std::endl;
	return 0;
}
