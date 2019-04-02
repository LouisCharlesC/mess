/**
 * @file printer.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "printer.h"

#include "broker.h"
#include "printertopics.h"
#include "sensortopics.h"

#include <iostream>
#include <sstream>

namespace mess
{
	template<>
	void onCall<Print>(Provider<Print>& provider)
	{
		provider.print();
	}
} // mess

void Printer::print()
{
		std::cout << "Sensor data: " << Broker::cache<SensorValueCache>().get() << std::endl;
}
