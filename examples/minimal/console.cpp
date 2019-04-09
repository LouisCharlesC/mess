/**
 * @file console.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-04-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "console.h"

#include "broker.h"
#include "interpretertopics.h"

#include <iostream>
#include <string>

// Define callback
namespace mess
{
	template<> template<>
	void Topic<Quit>::Notification::onNotify(Console& subscriber)
	{
		subscriber.quit();
	}
} // mess

Console::Console(Broker& broker):
	m_broker(broker)
{}

void Console::quit()
{
	m_quit = true;
}

void Console::loop()
{
	std::string input;
	while (!m_quit)
	{
		std::cin >> input;
		m_broker.call<Interpret>(input);
	}
}