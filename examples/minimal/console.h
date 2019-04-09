/**
 * @file console.h
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

class Console
{
public:
	Console(Broker& broker);
	void quit();
	void loop();

private:
	Broker& m_broker;
	bool m_quit = false;
};