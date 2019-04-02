/**
 * @file stdcin.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "stdcin.h"

#include "broker.h"
#include "printertopics.h"
#include "sensortopics.h"
#include "stdcinsubs.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

namespace mess
{
	// Subscription hooks
	template<>
	void onNotify<Quit, Stdcin>(Stdcin& subscriber)
	{
		subscriber.quit();
	}
} // mess

Stdcin::~Stdcin()
{
	m_thread.join();
}

void Stdcin::quit()
{
	m_quit = true;
}

void Stdcin::loop()
{
	std::stringbuf sb(std::ios::out);
	std::string str;

	while (!m_quit)
	{
		sb.str(str);
		std::cin.get(sb);

		if (!std::cin.eof())
		{
			std::cin.clear();
			std::cin.get();
			interpret(sb.str());
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(33));
		}
	}
}

void Stdcin::interpret(const std::string& input)
{
	if (input == "p")
	{
		Broker::call<Print>();
	}
	else if (input == "g")
	{
		std::cout << "Sensor period: " << Broker::call<GetPeriod>().count() << " ms" << std::endl;
	}
	else if (!input.empty() && input[0] == 's')
	{
		if (input.size() > 2)
		{
			mess::Return<GetPeriod>::rep period;
			std::istringstream iss(input.substr(2));
			iss >> period;
			if (iss)
			{
				Broker::call<SetPeriod>(mess::Return<GetPeriod>(period));
				std::cout << "Sensor period set to: " << Broker::call<GetPeriod>().count() << " ms" << std::endl;
			}
		}
	}
	else if (input == "q")
	{
		Broker::notify<Quit>();
	}
}