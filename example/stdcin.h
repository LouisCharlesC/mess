/**
 * @file stdcin.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "broker.h"
#include "controlmessages.h"
#include "stdcinmessages.h"

#include <iostream>

class Stdcin
{
public:
	void loop()
	{
		bool quit = false;
		while (!quit)
		{
			std::cin >> m_stdcinData;
			Broker::publish<StdcinTopic>(m_stdcinData);

			quit = m_stdcinData == "q";
			if (quit)
			{
				Broker::notify<QuitTopic>();
			}
		}
	}

private:
	StdcinTopic::Message m_stdcinData = "";
};