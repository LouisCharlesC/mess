/**
 * @file printer.h
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
#include "sensormessages.h"
#include "subscribemacros.h"

#include "state.hpp"

#include <iostream>

class Printer
{
public:
	void onPublish(const StdcinTopic::Message& stdcinData, StdcinTopic)
	{
		if (stdcinData == "p")
		{
			std::cout << "Sensor data: " << Broker::cache<SensorTopic>().get() << std::endl;
		}
	}

private:
	mess::State<SensorTopic::Message> m_sensorData;
};
MESS_SUBSCRIBE_TAG(Printer, StdcinTopic)