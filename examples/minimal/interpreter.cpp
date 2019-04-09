/**
 * @file interpreter.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-04-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "interpreter.h"

#include "broker.h"
#include "interpretersubs.h"

#include <iostream>
#include <string>

// Define callback
namespace mess
{
	template<>
	void Topic<Interpret>::Service::onCall(Interpreter& provider, const std::string& command)
	{
		provider.interpret(command);
	}
} // mess

Interpreter::Interpreter(Broker& broker):
	m_broker(broker)
{}

void Interpreter::interpret(const std::string& command)
{
	if (command == "q")
	{
		m_broker.notify<Quit>();
		std::cout << "Quit." << std::endl;
	}
	else
	{
		std::cout << "Echo: " << command << std::endl;
	}
}