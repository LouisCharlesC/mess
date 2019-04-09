/**
 * @file interpreter.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-04-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "broker.h"

#include <string>

class Interpreter
{
public:
	Interpreter(Broker& broker);
	void interpret(const std::string& command);

private:
	Broker& m_broker;
};